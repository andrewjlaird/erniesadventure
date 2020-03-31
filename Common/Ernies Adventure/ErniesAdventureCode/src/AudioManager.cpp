#include "AudioManager.h"
#include "CommonSettings.h"

#include <vector>
#include <iostream>

AudioManager::AudioManager()
 : musicOn(true),
   currentFileName(""),
   rampUp(false),
   rampDown(false),
   currentVolume(255),
   nextSong("")
{
}

AudioManager::~AudioManager()
{
}

////////////////////////////////////////////////////////////////////////////////

void AudioManager::loadMusic()
{
   std::vector<std::string> oggFiles = getdir("Music/", ".wav");

   for (int i = 0 ; i < oggFiles.size() ; i++)
   {
      SAMPLE* sample = load_sample(std::string("Music/" + oggFiles[i]).c_str());
      if (sample == NULL)
      {
         std::cout << "Couldn't load audio file: " << oggFiles[i] << std::endl;
      }
      else
      {
         soundSamplesMap[oggFiles[i]] = sample;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void AudioManager::changeBackgroundMusic(std::string newMusicName, bool forceStart)
{
   std::map<std::string, SAMPLE*>::iterator currentIter = soundSamplesMap.find(currentFileName);
   std::map<std::string, SAMPLE*>::iterator iter = soundSamplesMap.find(newMusicName);

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
               std::cout << "Ramping down before starting next song" << std::endl;
               rampUp = false;
               rampDown = true;
               nextSong = iter->first;
               std::cout << "Next song " << nextSong << std::endl;
            }
            else
            {
               std::cout << "Starting song now: " << iter->first << std::endl;
               nextSong = "";
               
               // (U) Start next song
               if (play_sample(iter->second, 255, 127, 1000, 1) < 0)
               {
                  std::cout << "Error playing file: " << iter->first << std::endl;
               }
               else
               {
                  currentFileName = iter->first;
               }
            }
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void AudioManager::toggleMusic()
{
   musicOn = !musicOn;
   
   if (musicOn)
   {
      std::cout << "turning music on for song: " << currentFileName << std::endl;
      std::map<std::string, SAMPLE*>::iterator iter = soundSamplesMap.find(currentFileName);
      
      if (iter != soundSamplesMap.end())
      {
         if (play_sample(iter->second, 0, 127, 1000, 1) < 0)
         {
            std::cout << "Error playing file: " << currentFileName << std::endl;
         }
         else
         {   
            rampDown = false;
            rampUp = true;
         }
      }
   }
   else
   {
      std::cout << "music ramping down for song: " << currentFileName << std::endl;

      // (U) Turn off music
      std::map<std::string, SAMPLE*>::iterator iter = soundSamplesMap.find(currentFileName);
      
      if (iter != soundSamplesMap.end())
      {
         rampUp = false;
         rampDown = true;
         nextSong = "";
      }
   }
}

void AudioManager::pollMusic()
{
   // Check if there's a ramp up / down going on and continue the ramp
   if (rampUp)
   {
      currentVolume += 5;
       
      std::map<std::string, SAMPLE*>::iterator iter = soundSamplesMap.find(currentFileName);
      adjust_sample(iter->second, currentVolume, 127, 1000, 1);
       
      if (currentVolume >= 255)
      {
         currentVolume = 255;
         rampUp = false;
      }
   }
   else if (rampDown)
   {
      currentVolume -= 5;
       
      std::map<std::string, SAMPLE*>::iterator iter = soundSamplesMap.find(currentFileName);
      adjust_sample(iter->second, currentVolume, 127, 1000, 1);
       
      if (currentVolume <= 0)
      {
         currentVolume = 0;
         rampDown = false;
         stop_sample(iter->second);
         rampDownComplete();
      }
   }
}

void AudioManager::rampDownComplete()
{
    std::cout << "Ramp down complete" << std::endl;
    std::cout << "Now starting song " << nextSong << " " << nextSong << std::endl;
    
   if (nextSong.compare("") != 0)
   {
      std::map<std::string, SAMPLE*>::iterator iter = soundSamplesMap.find(nextSong);
      
      if (iter != soundSamplesMap.end())
      {
         // Now that the ramp down is complete, start the next audio sample
         if (play_sample(iter->second, 255, 127, 1000, 1) < 0)
         {
            std::cout << "Error playing file: " << nextSong << std::endl;
         }
         else
         {
            currentFileName = nextSong;
            rampUp = true;
         }
      }
   }
}
