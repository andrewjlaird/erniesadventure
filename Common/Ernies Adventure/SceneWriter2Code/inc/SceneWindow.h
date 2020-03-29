#ifndef SCENE_WINDOW_H
#define SCENE_WINDOW_H

#include <vector>

#include "BaseWindow.h"
#include "Scene.h"
#include "DisplayDriver.h"
#include "SceneManager.h"
#include "CommonSettings.h"
#include "PathBrain.h"
#include "PathCharacter.h"

struct CurrentObjectMenuItem
{
   std::string objectName;
   unsigned long objectId;
};

class SceneWindow : public BaseWindow
{
   public:

      SceneWindow();
      
      ~SceneWindow();
      
      void reloadSceneObject(SceneIdType sceneObjectId);
      void reloadSprite(SpriteIdType spriteId);
      
      virtual void getNextFrame(BITMAP* frame);
      
      virtual void process();
      
      // (U) Bogus objects to make the scene objects happy
      DisplayDriver displayDriver;
      SceneManager sceneManager;
      /*SpriteManager spriteManager;
      VariableManager variableManager;
      Inventory inventory;
      DisplayManager displayManager;*/
      
   private:

      enum Mode
      {
         Border,
         JumpLine,
         JumpPointMode,
         ObjectPlacement,
         SceneMovement,
         Path,
         StartPoints
      };
      
      void mouseClickBorder();
      void mouseClickJumpLine();
      void mouseClickJumpPoint();
      void mouseClickObjectPlacement();
      void mouseClickSceneMovement();
      void mouseClickPath();
      void mouseClickStartPoints();

      void rightMouseClickBorder();
      void rightMouseClickJumpLine();
      void rightMouseClickJumpPoint();
      void rightMouseClickStartPoints();
      
      Point rightMouseClickDownPoint;
      
      void checkKeyboard();  
      
      Character* createCharacter(std::string characterName);
      
      void saveSceneGrid();
      void loadSceneGrid();
      void saveObjVarFile();
      std::vector<std::string> getAllCharacterGuiIds();

      void displayCharacterInfo(BITMAP* frame, int x, int width, int y);
      
      std::vector<std::string> makeObjectPlacementItems();
      std::vector<std::string> makeSpriteItems();
      std::vector<std::string> makeAllSpriteItems();
      std::vector<CurrentObjectMenuItem> makeObjectsInSceneItems();
      std::vector<CurrentObjectMenuItem> makeCharactersToFollowItems();
      std::vector<std::string> makePathSequenceNames();
      std::vector<std::string> makePathSequenceCommands();
      
      bool addSceneToGrid(int x, int y);
      bool removeSceneFromGrid(int x, int y);
      
      void moveSceneGridDown();
      void moveSceneGridRight();
      void addColumnToSceneGrid();
      void addRowToCurrentColumn();
      
      void moveSceneGridLeft();
      void moveSceneGridUp();

      void loadPathFile(std::string fileName);
      std::string pathCommandToString(PathCommand command);
      PathCommand stringToPathCommand(std::string command);
      void editCurrentPathItem();
      void addNewCommand();
      void savePathFile();
      Point determineRelativePoint(int x, int y);


      
      bool leftMenuSelected;
      std::vector<std::string> objectMenuItems;
      int objectMenuChoice;
      std::vector<CurrentObjectMenuItem> currentObjectMenuItems;
      int currentObjectMenuChoice;
      std::vector<std::string> spriteItems;
      
      Mode currentMode;
      
      BITMAP* blueMouse;
      BITMAP* orangeMouse;
      BITMAP* redMouse;
      BITMAP* purpleMouse;
      BITMAP* whiteMouse;
      BITMAP* greenMouse;
      BITMAP* blackMouse;
      
      int lastMouseZ;
      bool leftMouseDown;
      bool rightMouseDown;
      bool writeOn;
      Point lastClickedBorderPoint;
      Point lastClickedJumpLinePoint;
      Point lastClickedJumpPoint;

      // (U) Sprite info
      bool initialInGame;
      double framesPerSecond;
      int currentSpriteMenuChoice;
      
      // (U) Wander brain info
      int wanderRate;
      
      // (U) Follow and search brain info
      std::vector<CurrentObjectMenuItem> charactersToFollow;
      unsigned short selectedCharacterToFollow;
      CurrentObjectMenuItem characterToFollow;
      int distance;
      std::vector<std::string> avoidGuiIds;

      // Chase brain stuff
      double startMinTime;
      double startMaxTime;
      std::string startingText;
      std::string endingText;

      // (U) Character info
      bool initialMainCharacter;
      bool isTiedToScene;
      double pixelsPerSecond;
      BrainType brainType;
      int currentCharacterMenuChoice;
      CharacterIdType characterId;
      std::string characterGuiId;
      int currentRightSpriteChoice;
      int currentDownSpriteChoice;
      int currentLeftSpriteChoice;
      int currentUpSpriteChoice;
      int currentStopSpriteChoice;

      // Path stuff
      std::vector<std::string> pathSequenceFiles;
      int selectedPathSequenceFile;
      std::vector<std::pair<PathCommand, std::vector<std::string> > > commandsByStep;
      int selectedLoadedCommand;
      bool waitForStartPointClick;
      Point pathStartPoint;
      std::string pathFileName;
      bool waitForPathMouseClick;
      PathCommand commandToWaitFor;
      std::vector<std::string> pathSequenceCommands;
      int selectedPathCommand;
      PathCharacter* pathCharacter;
      std::string pathChangeSceneName;

      SceneIdType currentSceneId;
      Point currentJumpToPoint;
      
      std::vector<std::vector<Scene*> > sceneGrid;
      int selectedSceneX;
      int selectedSceneY;
      
      std::vector<std::string> currentSprites;
      
      int x;
      int y;

      bool keyDown;
      
      Character* newCharacter;
};

#endif
