#ifndef SCENE_H
#define SCENE_H

#include <allegro.h>
#ifdef _WIN32
#include <winalleg.h>
#endif
#include <string>
#include <vector>
#include <map>

#include "SceneSettings.h"
#include "SpriteSettings.h"
#include "CommonSettings.h"
#include "Action.h"
#include "Event.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
class AudioManager;
class LayeredObject;

class Scene
{
   public:
      
      Scene(SceneManager& sceneManager,
            SpriteManager& spriteManager,
            DisplayManager& displayManager,
            VariableManager& variableManager,
            AudioManager& audioManager,
            std::string sceneName,
            SceneIdType sceneId);
      
      ~Scene();
      
      /// (U) Loads up the scene elements except for the input which must be
      /// loaded after all scenes are loaded.
      ///
      /// @return True if successful
      bool loadScene();
      
      /// (U) Call this after loadScene because all scene objects needs loaded
      /// up before the input can be processed
      ///
      /// @return True if successful
      bool loadSceneInput();
      
      void setSceneOnScreen(bool isOnScreen);
      
      SceneIdType getSceneId();
      
      std::string getName();
      
      BITMAP* getBackgroundImage();
      
      void addCharacterById(int characterId, bool isTiedToScene);

      void removeCharacterById(CharacterIdType characterId);
      
      std::vector<unsigned long> getCharacterIds();
      
      // (U) Returns true, if this pixel is not in the boundary array.
      bool canMoveHere(int x, int y);
      
      std::vector<Point> getBoundaryVector();
      
      std::vector<short> getJumpLine();
      
      std::vector<JumpPoint> getJumpPoints();

      bool processInput();

      std::string getSaveString();

      void setEventHasExecuted(std::string eventId, bool hasExecuted);

      virtual void timeChange(double dt);

      // (U) Checks if the x y point is a jump point, and returns the scene to
      // jump to if it is.
      virtual JumpPoint isJumpPoint(int x, int y, bool checkLineOnly);
      
      // (U) Used after we jump into this scene to determine the starting point
      // of the character.
      // (U) The trigger point is the x and y coordinates that returned true when
      // passed to isJumpPoint.  The oldPoint is the x and y coordinates of the
      // character.
      virtual Point getJumpPoint(Point triggerPoint, Point oldPoint, int width, int height, bool follower);



      // (U) DEVELOPMENT
      void addBoundaryPoint(Point newPoint);
      void clearBoundaryPoint(int x, int y);
      void clearBoundaryPoints();

      void addJumpLinePoint(Point newPoint);
      void clearJumpLinePoint(int x, int y);
      void clearJumpLinePoints();

      void addJumpPoint(Point newPoint, SceneIdType sceneId, Point jumpToPoint);
      void clearJumpPoint(int x, int y);
      void clearJumpPoints();

      SceneIdType getLeftScene();
      SceneIdType getRightScene();
      SceneIdType getUpScene();
      SceneIdType getDownScene();

      void setLeftScene(SceneIdType sceneId);
      void setRightScene(SceneIdType sceneId);
      void setUpScene(SceneIdType sceneId);
      void setDownScene(SceneIdType sceneId);

      bool saveScene();

      const std::vector<Point>& getRandomStartPoints();
      void clearStartPoints();
      void addStartPoint(Point newPoint);
      
   private:
      // (U) The background bitmap
      BITMAP* background;

      // (U) An array of points that cannot be moved to.
      std::vector<Point> cannotMoveHere;

      // (U) Line used to determine where to jump to the scene north of here, and where to jump to
      // when coming from the scene north of here.
      std::vector<short> jumpLine;
      
      std::vector<JumpPoint> jumpPoints;

      std::vector<Point> randomStartPoints;
      
      std::vector<unsigned long> sceneCharacterIds;
      std::vector<unsigned long> rovingCharacterIds;
      std::vector<unsigned long> sceneSpriteIds;
      
      std::string sceneName;
      
      
      std::vector<Event*> sceneInputEvent;
      std::vector<Event*> nonInputEvent;
      
      

      SceneManager& sceneManager;
      SpriteManager& spriteManager;
      DisplayManager& displayManager;
      VariableManager& variableManager;
      AudioManager& audioManager;
      
      SceneIdType mySceneId;
      
      SceneIdType leftSceneId;
      SceneIdType rightSceneId;
      SceneIdType upSceneId;
      SceneIdType downSceneId;

      std::string musicFileName;
      
      bool loadCharacterFiles(std::ifstream& scenefile,
                              std::string sceneFolder,
                              std::string sceneName,
                              std::vector<Point> startPoints);

      bool loadSceneBorderFile(std::ifstream& scenefile);
                                
      bool loadSceneBoundaryFile(std::string boundaryFilePath);
      bool saveSceneBoundaryFile(std::string boundaryFilePath);
      
      void addCharacters(std::ofstream& sceneFile, std::vector<unsigned long> characterIds, bool tiedToScene);
     
};

#endif
