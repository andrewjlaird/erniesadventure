#ifndef SPRITE_SETTINGS_H
#define SPRITE_SETTINGS_H

#include <string>
#include <map>
#include <iostream>

typedef unsigned long SpriteIdType;
typedef unsigned long CharacterIdType;

enum BrainType
{
   InvalidBrainTypeEnum = 0,
   NullBrainEnum,
   WanderBrainEnum,
   KeyboardBrainEnum,
   FollowBrainEnum,
   SearchBrainEnum,
   AvoidBrainEnum,
   PathBrainEnum,
   ChaseBrainEnum,
   MaxBrainEnum
};

enum Direction
{
   Up = 0,
   Down = 1,
   Right = 2,
   Left = 3,
   Stop = 4,
   None = 5
};


#endif
