/* 
 * File:   AudioManager.h
 * Author: Lairds
 *
 * Created on July 18, 2011, 6:45 PM
 */

#ifndef AUDIOMANAGER_H
#define	AUDIOMANAGER_H

#include <allegro.h>
#ifdef _WIN32
#include <winalleg.h>
#endif

#ifdef BUILD_AUDIO
#include <almp3.h>
#endif

#include <string>
#include <map>


#ifdef BUILD_AUDIO
#define DATASZ  (1<<15) /* (32768) amount of data to read from disk each time */
#define BUFSZ   (1<<16) /* (65536) size of audiostream buffer */

typedef struct 
{
   PACKFILE *f;
   ALMP3_MP3STREAM *s;
} MP3FILE;
#endif

class AudioManager
{
    public:
        AudioManager();
        
        ~AudioManager();

        void loadMusic();
        void changeBackgroundMusic(std::string newMusicName, bool forceStart = false);
        void toggleMusic();
        void pollMusic();
        
    private:
        // Helper functions for handling mp3 streams
        #ifdef BUILD_AUDIO
        MP3FILE *open_mp3_file(const char *filename);
        int play_mp3_file(MP3FILE *mp3, int buflen, int vol, int pan);
        void close_mp3_file(MP3FILE *mp3);
        int poll_mp3_file(MP3FILE *mp3);
        
        // (U) Sound samples
        std::map<std::string, MP3FILE*> soundSamplesMap;
        #endif
        std::string currentFileName;
        bool musicOn;
};

#endif	/* AUDIOMANAGER_H */

