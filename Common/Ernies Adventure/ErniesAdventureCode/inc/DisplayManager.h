#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <vector>
#include <string>
#include <map>

#include <allegro.h>
#ifdef _WIN32
#include <winalleg.h>
#endif

#include "LayeredObject.h"
#include "SceneSettings.h"
#include "CommonSettings.h"

class DisplayDriver;
class SpriteManager;
class SceneManager;
class Inventory;
class VariableManager;
class AudioManager;

enum DialogType
{
   NewGame,
   GameOver
};

struct SkyColor
{
    int red;
    int green;
    int blue;
    int seconds;

    SkyColor(int inRed, int inGreen, int inBlue, int inSeconds)
     : red(inRed),
     green(inGreen),
     blue(inBlue),
     seconds(inSeconds)
    {

    }
};

class DisplayManager
{
   public:
      
      DisplayManager(DisplayDriver& inDisplayDriver,
                     SpriteManager& inSpriteManager,
                     SceneManager& inScreenManager,
                     Inventory& inventory,
                     VariableManager& variableManager,
                     AudioManager& audioManager);
      
      ~DisplayManager();
      
      // (U) 
      void run();

      void initLoadScreen(short numberOfUnits, int passColor, int failColor);
      void unitComplete(bool passed, std::string unitName);

      void endGame();

      void setUserInput(bool enabled);

      bool userInputDisabled();
      
      void displayText(std::vector<std::string> textToDisplay,
                       std::vector<std::string> inCharacterNames,
                       std::vector<std::string> options = std::vector<std::string>(),
                       DialogType dialogType = NewGame);

      void displayText(std::string textToDisplay,
                       std::string characterName);

      // (U) Merges the sprites and screenObjects on top of the background
      // and returns the merged bitmap.
      void mergeToBitmap(BITMAP* mergedBitmap,  // Populated in this call
                         BITMAP* background,
                         BITMAP* shadow,
                         std::vector<LayeredObject*> objects,
                         int xOffset = 0,
                         int yOffset = 0,
                         bool drawUI = true,
                         bool usePassedInValues = false,
                         std::vector<Point> boundaries = std::vector<Point>(),
                         std::vector<short> jumpLine = std::vector<short>(),
                         std::vector<JumpPoint> jumpPoints = std::vector<JumpPoint>());

      std::string getCurrentCommand();

      void scoreChange();

      bool stickyKeysOn();
      
      int getCharacterDirection();

   private:

      void updateLoadScreen(std::string unitName);

      // (U) Recolors the menu area and writes the menu items to the screen.
      void drawMenu(BITMAP* mergedBitmap,  // Populated in this call
                    std::vector<std::string> menuNames,
                    std::vector<std::string> menuItems,
                    int selectedIndex,
                    int xStartPixel);
      
      void displayTextWindow(BITMAP* mergedBitmap, //Populated in this call
                             std::string text,
                             std::string characterName);
      
      void handleUserInput(double dt);
      
      void menuItemSelected(std::string menuItemName);
      
      void loadFile(std::string fileName);
      void saveFile(std::string fileName);

      bool loadSkySettings();

      // (U) Referneces to the various managers and drivers
      DisplayDriver& displayDriver;
      SpriteManager& spriteManager;
      SceneManager& sceneManager;
      Inventory& inventory;
      VariableManager& variableManager;
      AudioManager& audioManager;

      // (U) Loading screen
      short loadedNumberOfUnits;
      int passColor;
      int failColor;
      std::vector<bool> unitSuccess;
      BITMAP* loadScreen;

      // (U) Set to true when thread should die
      bool wantToQuit;
      
      // (U) Speed of the game
      double speed;
      double elapsedTime;

      // (U) Input
      std::string currentCommand;
      int lastChar;
      bool keyDown;
      bool altDown;
      bool altUsed;

      // (U) Menu
      int selectedItem;
      int selectedMenu;
      bool menuDisplayed;
      bool menuAnimationFinished;
      double timeOnMenuAnimation;
      double animationRunTimeInMilliseconds;

      // (U) Score change
      bool scoreChangeActive;
      double timeOnScoreChangeColor;
      double scoreChangeTimeOut;

      // (U) Text window
      bool textWindowActive;
      std::vector<std::string> textToDisplay;
      int textSequenceNumber;
      std::vector<std::string> characterNames;

      // (U) Inventory
      bool displayInventory;
      double inventoryDisplayX;
      double inventoryMovementSpeed;
      double inventoryMaxMovementSpeedPps;
      double inventorySlowAmountPerSecondPpsPs;

      // (U) File IO
      bool loadFileWindowActive;
      bool saveFileWindowActive;
      int selectedSaveFile;
      int selectedLoadFile;
      std::vector<std::string> fileNames;
      std::string newFileName;

      // (U) Text Output
      bool dialogWindowActive;
      std::vector<std::string> options;
      int selectedResult;
      DialogType dialogType;

      bool uiDisabled;
      bool gameFinished;
      bool useStickyKeys;
      
      int characterDirection;

      // (U) Sky color settings
      std::vector<SkyColor> skyColors;
      BITMAP* duskShadow;

      std::vector< std::vector< std::string > > menus;
      std::vector<std::string> menuNames;
};

#endif
