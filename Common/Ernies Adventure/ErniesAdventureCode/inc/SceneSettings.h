#ifndef SCENE_SETTINGS_H
#define SCENE_SETTINGS_H

#include <string>
#include <map>

#include "CommonSettings.h"
#include "Action.h"
#include "Criteria.h"

typedef unsigned short SceneIdType;
typedef unsigned short EventIdType;

struct JumpPoint
{
   SceneIdType sceneId;
   Point jumpPoint;
   Point jumpToPoint;
   
   JumpPoint(SceneIdType inSceneId, Point inJumpPoint, Point inJumpToPoint)
    : sceneId(inSceneId),
      jumpPoint(inJumpPoint),
      jumpToPoint(inJumpToPoint)
   {
   }
};

#endif
