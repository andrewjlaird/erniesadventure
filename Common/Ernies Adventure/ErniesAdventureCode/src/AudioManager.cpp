#include "AudioManager.h"
#include "CommonSettings.h"

#include <vector>
#include <iostream>

AudioManager::AudioManager()
 : musicOn(true),
   currentFileName("")
{
}

AudioManager::~AudioManager()
{
}

#ifdef BUILD_AUDIO
MP3FILE* AudioManager::open_mp3_file(const char *filename) 
{
  MP3FILE *p = NULL;
  PACKFILE *f = NULL;
  ALMP3_MP3STREAM *s = NULL;
  char data[DATASZ];
  int len;

  if (!(p = (MP3FILE *)malloc(sizeof(MP3FILE))))
    goto error;
  if (!(f = pack_fopen(filename, F_READ)))
    goto error;
  if ((len = pack_fread(data, DATASZ, f)) <= 0)
    goto error;
  if (len < DATASZ) {
    if (!(s = almp3_create_mp3stream(data, len, TRUE)))
      goto error;
  }
  else {
    if (!(s = almp3_create_mp3stream(data, DATASZ, FALSE)))
      goto error;
  }
  p->f = f;
  p->s = s;
  return p;

  error:
  pack_fclose(f);
  free(p);
  return NULL;
}


int AudioManager::play_mp3_file(MP3FILE *mp3, int buflen, int vol, int pan) 
{
  return almp3_play_mp3stream(mp3->s, buflen, vol, pan);
}


void AudioManager::close_mp3_file(MP3FILE *mp3) 
{
  if (mp3) {
    pack_fclose(mp3->f);
    almp3_destroy_mp3stream(mp3->s);
    free(mp3);
  }
}


int AudioManager::poll_mp3_file(MP3FILE *mp3) 
{
  char *data;
  long len;

  data = (char *)almp3_get_mp3stream_buffer(mp3->s);
  if (data) {
    len = pack_fread(data, DATASZ, mp3->f);
    if (len < DATASZ)
      almp3_free_mp3stream_buffer(mp3->s, len);
    else
      almp3_free_mp3stream_buffer(mp3->s, -1);
  }

  return almp3_poll_mp3stream(mp3->s);
}
#endif

////////////////////////////////////////////////////////////////////////////////

void AudioManager::loadMusic()
{
#ifdef BUILD_AUDIO
   std::vector<std::string> mp3Files = getdir("Music/", ".mp3");

   for (int i = 0 ; i < mp3Files.size() ; i++)
   {
      MP3FILE* newMp3 = open_mp3_file(std::string("Music/" + mp3Files[i]).c_str());

      if (!newMp3)
      {
         std::cout << "Couldn't load mp3 file: " << mp3Files[i] << std::endl;
      }
      else
      {
         soundSamplesMap[mp3Files[i]] = newMp3;
      }
   }
#endif
}

////////////////////////////////////////////////////////////////////////////////

void AudioManager::changeBackgroundMusic(std::string newMusicName, bool forceStart)
{
#ifdef BUILD_AUDIO
   std::map<std::string, MP3FILE*>::iterator currentIter = soundSamplesMap.find(currentFileName);
   std::map<std::string, MP3FILE*>::iterator iter = soundSamplesMap.find(newMusicName);

   if ((iter != soundSamplesMap.end() &&
       currentIter != soundSamplesMap.end()) ||
       (iter != soundSamplesMap.end() &&
       currentFileName.compare("") == 0))
   {
      if (iter->first.compare(currentFileName) != 0 || forceStart)
      {
         if (musicOn)
         {
            // (U) Stop current music
            if (currentFileName.compare("") != 0)
            {
               //voice_ramp_volume(currentVoice, 1000, 0);
   
               // (U) Start next song
               almp3_stop_mp3stream(currentIter->second->s);
               if (play_mp3_file(iter->second, BUFSZ, 255, 128) != ALMP3_OK)
               {
                  std::cout << "Error playing file: " << iter->first << std::endl;
               }
               //stop_sample(iter->second);
               //currentVoice = play_sample(iter->second, 0, 127, 1000, 1);
               //voice_ramp_volume(currentVoice, 3000, 255);
            }
            else
            {
               // (U) Start next song
               if (play_mp3_file(iter->second, BUFSZ, 255, 128) != ALMP3_OK)
               {
                  std::cout << "Error playing file: " << iter->first << std::endl;
               }
               //currentVoice = play_sample(iter->second, 255, 127, 1000, 1);
            }
         }
         
         currentFileName = iter->first;
      }
   }
#endif
}

////////////////////////////////////////////////////////////////////////////////

void AudioManager::toggleMusic()
{
   musicOn = !musicOn;
   
   if (musicOn)
   {
      std::cout << "turning music on" << std::endl;
      changeBackgroundMusic(currentFileName, true);
   }
   else
   {
      std::cout << "music ramping down" << std::endl;
      // (U) Turn off music
      
#ifdef BUILD_AUDIO
      std::map<std::string, MP3FILE*>::iterator iter = soundSamplesMap.find(currentFileName);
      
      if (iter != soundSamplesMap.end())
      {      
         //voice_ramp_volume(currentVoice, 1000, 0);
         close_mp3_file(iter->second);
      }
#endif
   }
}

void AudioManager::pollMusic()
{
#ifdef BUILD_AUDIO
   std::map<std::string, MP3FILE*>::iterator iter = soundSamplesMap.find(currentFileName);

   if (iter != soundSamplesMap.end())
   {      
      if (poll_mp3_file(iter->second) != ALMP3_OK)
      {
         almp3_destroy_mp3stream(iter->second->s);
                 
         iter->second = open_mp3_file(std::string("Music/" + iter->first).c_str());
         
         if (play_mp3_file(iter->second, BUFSZ, 255, 128) != ALMP3_OK)
         {
            std::cout << "Error playing file: " << iter->first << std::endl;
         }
      }
   }
#endif
}
