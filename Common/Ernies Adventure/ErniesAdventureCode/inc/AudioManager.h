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

#include <string>
#include <map>


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
        void rampDownComplete();
        
        // (U) Sound samples
        std::map<std::string, SAMPLE*> soundSamplesMap;
        std::string currentFileName;
        bool musicOn;
        bool rampUp;
        bool rampDown;
        int currentVolume;
        std::string nextSong;
};

#endif	/* AUDIOMANAGER_H */

