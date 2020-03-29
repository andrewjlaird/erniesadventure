#include "DisplayManager.h"

#include "DisplayDriver.h"
#include "SpriteManager.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "Character.h"
#include "Sprite.h"

#include "DisplaySettings.h"
#include "GlobalTimer.h"
#include "CommonSettings.h"
#include "Inventory.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <math.h>

DisplayManager::DisplayManager(DisplayDriver& inDisplayDriver,
                               SpriteManager& inSpriteManager,
                               SceneManager& inSceneManager,
                               Inventory& inInventory,
                               VariableManager& inVariableManager,
                               AudioManager& inAudioManager)
 : displayDriver(inDisplayDriver),
   spriteManager(inSpriteManager),
   sceneManager(inSceneManager),
   inventory(inInventory),
   variableManager(inVariableManager),
   audioManager(inAudioManager),
   wantToQuit(false),
   speed(1.0),
   elapsedTime(0.0),
   currentCommand(""),
   lastChar(0),
   keyDown(false),
   altDown(false),
   altUsed(false),
   selectedItem(0),
   selectedMenu(0),
   menuDisplayed(false),
   menuAnimationFinished(true),
   timeOnMenuAnimation(0),
   animationRunTimeInMilliseconds(500),
   scoreChangeActive(false),
   timeOnScoreChangeColor(0),
   scoreChangeTimeOut(5),
   textWindowActive(false),
   textToDisplay(),
   textSequenceNumber(0),
   characterNames(),
   displayInventory(false),
   inventoryDisplayX(0),
   inventoryMovementSpeed(0),
   inventoryMaxMovementSpeedPps(100),
   inventorySlowAmountPerSecondPpsPs(70),
   loadFileWindowActive(false),
   saveFileWindowActive(false),
   selectedSaveFile(0),
   selectedLoadFile(0),
   newFileName(""),
   dialogWindowActive(false),
   selectedResult(0),
   dialogType(NewGame),
   gameFinished(false),
   uiDisabled(false),
   useStickyKeys(true),
   characterDirection(0)
{
   // (U) Create the file menus
   menuNames.push_back("File");
   menuNames.push_back("Options");
   menuNames.push_back("Help");
   
   std::vector<std::string> fileMenu;
   fileMenu.push_back("New .... Ctrl - N");
   fileMenu.push_back("Open ... Ctrl - O");
   fileMenu.push_back("Save ... Ctrl - S");
   fileMenu.push_back("");
   fileMenu.push_back("Exit");

   menus.push_back(fileMenu);


   std::vector<std::string> optionsMenu;
   optionsMenu.push_back("Speed Up .............. Ctrl - =");
   optionsMenu.push_back("Slow Down ............. Ctrl - -");
   optionsMenu.push_back("Toggle Full Screen .... Alt - Enter");
   optionsMenu.push_back("Toggle Sticky Keys");
   
#ifdef BUILD_AUDIO
   optionsMenu.push_back("Toggle Music .......... F11");
#endif

   menus.push_back(optionsMenu);


   std::vector<std::string> helpMenu;
   helpMenu.push_back("Help");
   helpMenu.push_back("About");

   menus.push_back(helpMenu);
   
   setup_timer();

   if (!loadSkySettings())
   {
      std::cerr << "ERROR: Could not load sky settings file" << std::endl;
   }
   
   duskShadow = create_bitmap(640, 480);
   rectfill(duskShadow, 0, 0, 640, 480, makecol(0, 0, 0));
}

////////////////////////////////////////////////////////////////////////////////

DisplayManager::~DisplayManager()
{
}

////////////////////////////////////////////////////////////////////////////////
 
void DisplayManager::run()
{
   int frames_this_second = 0;
   double one_second = 0;

   BITMAP* mergedBitmap = create_bitmap(screenWidth, screenHeight);

   display_last_time = 0;
   display_elapsed_time = 0;
   display_dt = 0;

   while (!wantToQuit)
   {
      // (U) Give the CPU some rest
      if (!debugRestOff)
      {
         rest(50);
      }
      
      audioManager.pollMusic();
         
      frames_this_second++;
        
      display_dt = display_elapsed_time - display_last_time;
      display_last_time = display_elapsed_time;
      one_second += display_dt;
            
      double adjustedDt = display_dt * speed;
            
      if (menuDisplayed)
      {
         timeOnMenuAnimation += (display_dt * 1000);
      }
      else if(displayInventory)
      {
         if (inventoryMovementSpeed > 0)
         {
            inventoryMovementSpeed -= inventorySlowAmountPerSecondPpsPs * display_dt;
         }
         else if (inventoryMovementSpeed < 0)
         {
            inventoryMovementSpeed += inventorySlowAmountPerSecondPpsPs * display_dt;
         }
         
         inventoryDisplayX += inventoryMovementSpeed * display_dt;
      }
      else if(!textWindowActive && !gameFinished && !displayInventory && !loadFileWindowActive && !saveFileWindowActive && !dialogWindowActive)
      {
         // (U) Convert to milliseconds
         spriteManager.timeChange(adjustedDt * 1000);
         sceneManager.timeChange(adjustedDt * 1000);

         elapsedTime += adjustedDt;
      }

      if (scoreChangeActive)
      {
         timeOnScoreChangeColor += display_dt;

         if (timeOnScoreChangeColor >= scoreChangeTimeOut)
         {
            scoreChangeActive = false;
            timeOnScoreChangeColor = 0;
         }
      }

      handleUserInput(adjustedDt * 1000);

      int totalSeconds = 0;
      int red = 0;
      int green = 0;
      int blue = 0;

      double timeScaled = elapsedTime;

      for (int i = 0 ; i < skyColors.size() ; i++)
      {
         if (timeScaled >= totalSeconds && timeScaled < totalSeconds + skyColors[i].seconds && i < skyColors.size() - 1)
         {
            double intoSeconds = timeScaled - totalSeconds;
            double dTimeIn = intoSeconds / skyColors[i].seconds;

            red = static_cast<int>(skyColors[i].red + (skyColors[i + 1].red - skyColors[i].red) * dTimeIn);
            green = static_cast<int>(skyColors[i].green + (skyColors[i + 1].green - skyColors[i].green) * dTimeIn);
            blue = static_cast<int>(skyColors[i].blue + (skyColors[i + 1].blue - skyColors[i].blue) * dTimeIn);

            break;
         }

         totalSeconds += skyColors[i].seconds;

         if (i == skyColors.size() - 1)
         {
            red = skyColors[skyColors.size() - 1].red;
            green = skyColors[skyColors.size() - 1].green;
            blue = skyColors[skyColors.size() - 1].blue;
         }
      }

      double factor = 1.0;

      for (int i = 0 ; i < 480 ; i++)
      {
         int adjustedRed = static_cast<int>(red * factor);
         int adjustedGreen = static_cast<int>(green * factor);
         int adjustedBlue = static_cast<int>(blue * factor);
         line(mergedBitmap, 0, i, 640, i, makecol(static_cast<int>(adjustedRed), static_cast<int>(adjustedGreen), static_cast<int>(adjustedBlue)));

         factor -= 0.001;
      }
      
      // (U) Get sorted set of layered objects on screen
      std::vector<LayeredObject*> sprites = spriteManager.getLayeredObjectsOnScreen();

      mergeToBitmap(mergedBitmap,
                    sceneManager.getBackgroundImage(),
                    sceneManager.getShadowLayer(),
                    sprites);

      displayDriver.updateDisplay(mergedBitmap, !debugVsyncOff);
      
      if( one_second > 1 )
      {
         display_frames_per_second = frames_this_second;
                        
         one_second = 0;
         frames_this_second = 0;
      }
   }

   destroy_bitmap(mergedBitmap);
}

////////////////////////////////////////////////////////////////////////////////

void DisplayManager::initLoadScreen(short numberOfUnits, int inPassColor, int inFailColor)
{
   loadedNumberOfUnits = numberOfUnits;
   passColor = inPassColor;
   failColor = inFailColor;
   unitSuccess.clear();

   loadScreen = load_bitmap("LoadScreen.bmp", NULL);

   updateLoadScreen("");
}

////////////////////////////////////////////////////////////////////////////////

void DisplayManager::unitComplete(bool passed, std::string unitName)
{
   unitSuccess.push_back(passed);

   updateLoadScreen(unitName);
}

////////////////////////////////////////////////////////////////////////////////

void DisplayManager::endGame()
{
   gameFinished = true;

   std::vector<std::string> text;
   text.push_back("Thanks for playing Ernie's Adventure!");

   std::vector<std::string> options;
   options.push_back("New Game");
   options.push_back("Load Game");
   options.push_back("Quit");

   displayText(text, std::vector<std::string>(), options, GameOver);
}

////////////////////////////////////////////////////////////////////////////////

void DisplayManager::setUserInput(bool enabled)
{
   uiDisabled = !enabled;
}

////////////////////////////////////////////////////////////////////////////////

bool DisplayManager::userInputDisabled()
{
   return uiDisabled;
}

////////////////////////////////////////////////////////////////////////////////

void DisplayManager::displayText(std::vector<std::string> inTextToDisplay,
                                 std::vector<std::string> inCharacterNames,
                                 std::vector<std::string> inOptions,
                                 DialogType inDialogType)
{
   textToDisplay = inTextToDisplay;
   characterNames = inCharacterNames;
   textSequenceNumber = 0;

   options.clear();

   if (inOptions.size() > 0)
   {
      selectedResult = 0;
      options = inOptions;
      dialogType = inDialogType;
      dialogWindowActive = true;
   }
   else
   {
      textWindowActive = true;
   }
}

////////////////////////////////////////////////////////////////////////////////

void DisplayManager::displayText(std::string inTextToDisplay,
                                 std::string characterName)
{
   textToDisplay.clear();
   textToDisplay.push_back(inTextToDisplay);
   characterNames.clear();
   characterNames.push_back(characterName);
   textSequenceNumber = 0;

   options.clear();

   textWindowActive = true;
}

////////////////////////////////////////////////////////////////////////////////

std::string DisplayManager::getCurrentCommand()
{
    return currentCommand;
}

////////////////////////////////////////////////////////////////////////////////

void DisplayManager::scoreChange()
{
   scoreChangeActive = true;
   timeOnScoreChangeColor = 0;
}

////////////////////////////////////////////////////////////////////////////////

bool DisplayManager::stickyKeysOn()
{
   return useStickyKeys;
}

////////////////////////////////////////////////////////////////////////////////

int DisplayManager::getCharacterDirection()
{
    int tmpDirection = characterDirection;
    
    if (!keyDown)
    {
       characterDirection = 0;
    }
    
    return tmpDirection;
}

////////////////////////////////////////////////////////////////////////////////

void DisplayManager::updateLoadScreen(std::string unitName)
{
   BITMAP* mergedBitmap = create_bitmap(640, 480);

   if (mergedBitmap != NULL && loadScreen != NULL)
   {
      draw_sprite(mergedBitmap, loadScreen, 0, 0);

      rect(mergedBitmap, 300, 415, 600, 430, makecol(41, 69, 33));

      if (loadedNumberOfUnits > 0)
      {
         double unitWidth = 298.0 / loadedNumberOfUnits;

         for (int i = 0 ; i < unitSuccess.size() ; i++)
         {
            if (unitSuccess[i])
            {
               if (i < loadedNumberOfUnits - 1)
               {
                  rectfill(mergedBitmap,
                           static_cast<int>(301 + i * unitWidth),
                           416,
                           static_cast<int>(301 + ((i + 1) * unitWidth)),
                           429,
                           passColor);
               }
               else
               {
                  rectfill(mergedBitmap,
                           static_cast<int>(301 + i * unitWidth),
                           416,
                           599,
                           429,
                           passColor);
               }
            }
            else
            {
               if (i < loadedNumberOfUnits - 1)
               {
                  rectfill(mergedBitmap,
                           static_cast<int>(301 + i * unitWidth),
                           416,
                           static_cast<int>(301 + ((i + 1) * unitWidth)),
                           429,
                           failColor);
               }
               else
               {
                  rectfill(mergedBitmap,
                           static_cast<int>(301 + i * unitWidth),
                           416,
                           599,
                           429,
                           failColor);
               }
            }
         }
      }

      textprintf_ex(mergedBitmap,
                    font, 300, 440, makecol(200, 200, 200),
                    -1, "Loading... %d%%", static_cast<int>(unitSuccess.size() / static_cast<double>(loadedNumberOfUnits) * 100));

      textprintf_ex(mergedBitmap,
                    font, 300, 450, makecol(200, 200, 200),
                    -1, "%s", unitName.c_str());

      displayDriver.updateDisplay(mergedBitmap, true);
   }
}

////////////////////////////////////////////////////////////////////////////////

void DisplayManager::mergeToBitmap(BITMAP* mergedBitmap,
                                   BITMAP* background,
                                   BITMAP* shadow,
                                   std::vector<LayeredObject*> objects,
                                   int xOffset,
                                   int yOffset,
                                   bool drawUI,
                                   bool usePassedInValues,
                                   std::vector<Point> boundaries,
                                   std::vector<short> jumpLine,
                                   std::vector<JumpPoint> jumpPoints)
{
   if (background != NULL)
   {   
      // (U) Place background
      draw_sprite( mergedBitmap, background, xOffset + 0, yOffset + 0);
   }

   if (shadow != NULL)
   {
      // (U) Draw shadow but get it lighter as time passes
      int maxShadowLevel = 50;
      int shadowLevel = static_cast<int>(maxShadowLevel - (elapsedTime / 2000 * maxShadowLevel));
   
      if (shadowLevel < 0)
      {
         shadowLevel = 0;
      }
   
      set_trans_blender(0, 0, 0, shadowLevel);
   
      // (U) Place shadow
      draw_trans_sprite(mergedBitmap, shadow, xOffset + 0, yOffset + 0);
   }

   // (U) Place the objects on the screen
   for(int i = 0 ; i < objects.size() ; i++)
   {
      if (objects[i] != NULL)
      {
         BITMAP* image = objects[i]->getCurrentImage();

         if (image != NULL)
         {
            // Draw image to bitmap
            draw_sprite(mergedBitmap, image, xOffset + objects[i]->getX(), yOffset + objects[i]->getY());
            std::string name = objects[i]->getOnScreenName();
            
            if (name.compare("") != 0)
            {
               int mainX = spriteManager.getMainCharacterX();
               int mainY = spriteManager.getMainCharacterY();

               Square objOffset = objects[i]->getOffsetBoundarySquare();

               short myX = objOffset.lowerRight.x;
               short myY = (objOffset.upperLeft.y - objOffset.lowerRight.y) / 2 + objOffset.upperLeft.y;

               int deltaX = mainX - int(myX);
               int deltaY = mainY - int(myY);
               
               double dist = sqrt(deltaX * deltaX + deltaY * deltaY);
               int text_intensity = 0;
               
               if (dist < 150 && dist > 1)
               {
                  text_intensity = int(255 - (dist / 150 * 255));
               }
               else if (dist <= 1)
               {
                   text_intensity = 255;
               }
               
               set_trans_blender(0, 0, 0, text_intensity);
               BITMAP* textBitmap = create_bitmap(100, 10);
               clear_to_color(textBitmap, makecol(255,0,255));
               textprintf_ex(textBitmap, font, 0, 0, makecol(255, 255, 255), -1, "%s", name.c_str());
               draw_trans_sprite(mergedBitmap, textBitmap, xOffset + objects[i]->getX(), yOffset + objects[i]->getY() - 7);
               destroy_bitmap(textBitmap);
            }
         }
      }
   }

   int maxTransLevel = 100;
   int transLevel = static_cast<int>(maxTransLevel * ((elapsedTime - 2000) / 5000));

   if (transLevel > maxTransLevel)
   {
      transLevel = maxTransLevel;
   }
   else if (transLevel < 0)
   {
      transLevel = 0;
   }

   set_trans_blender(0, 0, 0, transLevel);

   // (U) Place shadow
   draw_trans_sprite(mergedBitmap, duskShadow, xOffset + 0, yOffset + 0);

   if (drawUI)
   {
      // (U) Recolor the lower 13 pixels so that the text stands out
      for (int i = 467 ; i < 480 ; i++)
      {
         for (int j = 0 ; j < 640 ; j++)
         {
            int color = getpixel(mergedBitmap, j, i);
            
            int red = getr32(color);
            int green = getg32(color);
            int blue = getb32(color);
            
            putpixel(mergedBitmap, j, i, makecol(red / 3, green / 3, blue / 3));
         }
      }
      
      textprintf_ex(mergedBitmap, font, 5, 470, makecol(255, 255, 255), -1, "> %s", currentCommand.c_str());
      //textprintf_ex(mergedBitmap, font, 600, 470, makecol(255, 255, 255), -1, "%d", lastChar);

      // (U) Recolor the upper 13 pixels so that the text stands out
      for (int i = 0 ; i < 13 ; i++)
      {
         for (int j = 0 ; j < 640 ; j++)
         {
            int color = getpixel(mergedBitmap, j, i);
            
            int red = getr32(color);
            int green = getg32(color);
            int blue = getb32(color);
            
            putpixel(mergedBitmap, j, i, makecol(red / 3, green / 3, blue / 3));
         }
      }
      
      textprintf_ex(mergedBitmap, font, 500, 3, makecol(255, 255, 255), -1, "Ernie's Adventure", currentCommand.c_str());

      int hours = static_cast<int>(6 + (elapsedTime / 60 / 60));
      int minutes = static_cast<int>(elapsedTime / 60) % 60;
      int seconds = static_cast<int>(elapsedTime) % 60;

      textprintf_ex(mergedBitmap, font, 250, 3, makecol(255, 255, 255), -1, "%d:%0.2d:%0.2d", hours, minutes, seconds);

      if (scoreChangeActive && static_cast<int>(timeOnScoreChangeColor) % 2 == 0)
      {
         textprintf_ex(mergedBitmap, font, 380, 3, makecol(255, 100, 100), -1, "Score %d of 49", variableManager.getScore());
      }
      else
      {
         textprintf_ex(mergedBitmap, font, 380, 3, makecol(255, 255, 255), -1, "Score %d of 49", variableManager.getScore());
      }
      
      if (menuDisplayed)
      {
         int startingX = 0;
         
         for (int i = 0 ; i < selectedMenu ; i++)
         {
            startingX += ((menuNames[i].size() + 2) * 8);
         }
         
         drawMenu(mergedBitmap, menuNames, menus[selectedMenu], selectedItem, startingX);
      }
      
      // (U) Draw file menu either way
      int characterPos = 0;
      
      for (int i = 0 ; i < menuNames.size() ; i++)
      {
         textprintf_ex(mergedBitmap, font, 2 + characterPos * 8, 2, makecol(255, 255, 255), -1, "%s", menuNames[i].c_str());   
         
         characterPos += menuNames[i].size() + 2;
      }
      
      if ((textWindowActive || dialogWindowActive) && textSequenceNumber < textToDisplay.size())
      {
         if (dialogWindowActive)
         {
            displayTextWindow(mergedBitmap, textToDisplay[textSequenceNumber], "");
         }
         else
         {
            std::string characterName = "";

            if (textSequenceNumber < characterNames.size())
            {
               characterName = characterNames[textSequenceNumber];
            }

            displayTextWindow(mergedBitmap, textToDisplay[textSequenceNumber], characterName);
         }
      }
      else if((textWindowActive || dialogWindowActive) && textSequenceNumber >= textToDisplay.size())
      {
         std::cerr << "ERROR: Text sequence number is out of bounds" << std::endl;
         textSequenceNumber = 0;
         textToDisplay.clear();
         characterNames.clear();
         textWindowActive = false;
         dialogWindowActive = false;
      }
      
      if (displayInventory)
      {
         int areaWidth = 420;
         int areaHeight = 75;
         
         BITMAP* inventoryBitmap = create_bitmap(areaWidth, areaHeight);
         
         // (U) Recolor the inventory area in the center of the screen
         for (int i = (screenWidth - areaWidth) / 2, x = 0 ; i < (screenWidth - areaWidth) / 2 + areaWidth ; i++, x++)
         {
            for (int j = (screenHeight - areaHeight) / 2, y = 0 ; j < (screenHeight - areaHeight) / 2 + areaHeight ; j++, y++)
            {
               int color = getpixel(mergedBitmap, i, j);
               
               int red = getr32(color);
               int green = getg32(color);
               int blue = getb32(color);
               
               putpixel(inventoryBitmap, x, y, makecol(red / 3, green / 3, blue / 3));
            }
         }
         
         std::vector<InventoryObject> inventoryObjects = inventory.getInventory();

         unsigned short startStopOffset = 10;
         unsigned short gap = 10;
         unsigned short currentX = 0;

         for (int i = 0 ; i < inventoryObjects.size() ; i++)
         {
            int itemWidth = 0;

            if (inventoryObjects[i].image->w > inventoryObjects[i].name.size() * 8)
            {
               itemWidth = inventoryObjects[i].image->w;
            }
            else
            {
               itemWidth = inventoryObjects[i].name.size() * 8;
            }

            // (U) Only draw it if it's partially visible
            if (static_cast<int>(inventoryDisplayX) + itemWidth + currentX > 0 &&
                static_cast<int>(inventoryDisplayX) + itemWidth + currentX < areaWidth + itemWidth)
            {
               draw_sprite(inventoryBitmap,
                       inventoryObjects[i].image,
                       static_cast<int>(inventoryDisplayX) + startStopOffset + currentX + itemWidth / 2 - inventoryObjects[i].image->w / 2,
                       5);
               textprintf_centre_ex(inventoryBitmap,
                       font,
                       static_cast<int>(inventoryDisplayX) + startStopOffset + currentX + itemWidth / 2,
                       55,
                       makecol(120, 120, 120),
                       -1,
                       "%s",
                       inventoryObjects[i].name.c_str());
            }

            currentX += itemWidth + gap;
         }
         
         draw_sprite(mergedBitmap, inventoryBitmap, (screenWidth - areaWidth) / 2, (screenHeight - areaHeight) / 2);

         // (U) Stop movement if we're off the edge
         if ((currentX + startStopOffset - areaWidth) < 0 || inventoryDisplayX > 0)
         {
            inventoryDisplayX = 0;
            inventoryMovementSpeed = 0;
         }
         else if (inventoryDisplayX < (currentX + startStopOffset - areaWidth) * -1)
         {
            inventoryDisplayX = (currentX + startStopOffset - areaWidth) * -1;
            inventoryMovementSpeed = 0;
         }
      }
      
      if (loadFileWindowActive)
      {
         int width = 0;
         int height = 0;

         if (fileNames.size() > 0)
         {
            for (int i = 0 ; i < fileNames.size() ; i++)
            {
               if (fileNames[i].length() * 8 > width)
               {
                  width = fileNames[i].length() * 8;
               }
            }
            
            width += 16;
            
            height = fileNames.size() * 10;
            
            recoloredArea(mergedBitmap, 320 - (width / 2), 240 - (height / 2), 320 + (width / 2), 240 + (height / 2));
            
            for (int i = 0 ; i < fileNames.size() ; i++)
            {
               textprintf_ex(mergedBitmap, font, 320 - (width / 2) + 1, 240 - (height / 2) + i * 10 + 1, makecol(200, 200, 200), -1, "%s%s", (selectedLoadFile == i ? "> " : "  "), fileNames[i].c_str());
            }
         }
         else
         {
            width = 27 * 8;
            height = 10;
            
            recoloredArea(mergedBitmap, 320 - (width / 2), 240 - (height / 2), 320 + (width / 2), 240 + (height / 2));
            textprintf_ex(mergedBitmap, font, 320 - (width / 2) + 1, 240 - (height / 2) + 1, makecol(200, 200, 200), -1, "No Saved Game Files To Load");
         }
      }
      else if(saveFileWindowActive)
      {
         int width = 0;
         
         for (int i = 0 ; i < fileNames.size() ; i++)
         {
            if (fileNames[i].length() * 8 > width)
            {
               width = fileNames[i].length() * 8;
            }
         }

         if ((newFileName.length() + 4) * 8 > width)
         {
            width = (newFileName.length() + 4) * 8;
         }

         if (newFileName.length() == 0 && 15 * 8 > width)
         {
            width = 15 * 8;
         }

         width += 16;
         
         int height = (fileNames.size() + 1) * 10;
         
         recoloredArea(mergedBitmap, 320 - (width / 2), 240 - (height / 2), 320 + (width / 2), 240 + (height / 2));
         
         for (int i = 0 ; i < fileNames.size() ; i++)
         {
            textprintf_ex(mergedBitmap, font, 320 - (width / 2) + 1, 240 - (height / 2) + i * 10 + 1, makecol(200, 200, 200), -1, "%s%s", (selectedSaveFile == i ? "> " : "  "), fileNames[i].c_str());
         }
         
         textprintf_ex(mergedBitmap, 
                       font, 
                       320 - (width / 2) + 1, 
                       240 - (height / 2) + fileNames.size() * 10 + 1, 
                       makecol(200, 200, 200), 
                       -1, 
                       "%s%s", 
                       (selectedSaveFile == fileNames.size() ? "> " : "  "), 
                       (newFileName.length() > 0 ? std::string(newFileName + ".svg").c_str() : "<New File Name>"));
      }
   }
   
   
   
   // (U) Debug
   int debugY = 15;

   if (debugFpsOn)
   {
      textprintf_ex(mergedBitmap, font, 5, debugY, makecol(255, 0, 0), -1, "fps: %d", display_frames_per_second);
      textprintf_ex(mergedBitmap, font, 5, debugY + 40, makecol(255, 0, 0), -1, "speed: %f", speed);
   }
   
   if (debugVsyncOff)
   {
      textprintf_ex(mergedBitmap, font, 5, debugY + 10, makecol(255, 0, 0), -1, "vsync off");
   }
   
   if (debugRestOff)
   {
      textprintf_ex(mergedBitmap, font, 5, debugY + 20, makecol(255, 0, 0), -1, "rest off");
   }
   
   if (debugXYOn)
   {
      textprintf_ex(mergedBitmap, font, 5, debugY + 30, makecol(255, 0, 0), -1, "%d %d", spriteManager.getMainCharacterX(), spriteManager.getMainCharacterY());
   }
   
   if (debugBordersOn || usePassedInValues)
   {      
      if (usePassedInValues)
      {
         for (int i = 0 ; i < boundaries.size() ; i++)
         {
            putpixel( mergedBitmap, xOffset + boundaries[i].x, yOffset + boundaries[i].y, makecol(0,0,255));
         }
         
         for (int i = 0 ; i < jumpLine.size() ; i++)
         {
            putpixel( mergedBitmap, xOffset + i, yOffset + jumpLine[i], makecol(255,160,0));
         }
         
         for (int i = 0 ; i < jumpPoints.size() ; i++)
         {
            putpixel( mergedBitmap, xOffset + jumpPoints[i].jumpPoint.x, yOffset + jumpPoints[i].jumpPoint.y, makecol(128,0,128));
         }
      }
      else
      {
         std::vector<Point> boundaries = sceneManager.getCurrentBoundaryVector();
      
         for (int i = 0 ; i < boundaries.size() ; i++)
         {
            putpixel( mergedBitmap, xOffset + boundaries[i].x, yOffset + boundaries[i].y, makecol(0,0,255));
         }
         
         std::vector<short> jumpLine = sceneManager.getCurrentJumpLine();
         
         for (int i = 0 ; i < jumpLine.size() ; i++)
         {
            putpixel( mergedBitmap, xOffset + i, yOffset + jumpLine[i], makecol(255,160,0));
         }
         
         std::vector<JumpPoint> jumpPoints = sceneManager.getCurrentJumpPoints();
         
         for (int i = 0 ; i < jumpPoints.size() ; i++)
         {
            putpixel( mergedBitmap, xOffset + jumpPoints[i].jumpPoint.x, yOffset + jumpPoints[i].jumpPoint.y, makecol(128,0,128));
         }
      }
      
      for (int j = 0 ; j < objects.size() ; j++)
      {
         if (objects[j] != NULL)
         {
            std::vector<short> heights = objects[j]->getHeightVector();

            for (int i = 0 ; i < heights.size() ; i++)
            {
               if (heights[i] != -1)
               {
                  putpixel( mergedBitmap, xOffset + objects[j]->getX() + i, yOffset + objects[j]->getY() + heights[i], makecol(255,0,0));
               }
            }

            Sprite* sprite = NULL;

            if (sprite = dynamic_cast<Sprite*>(objects[j]))
            {
               putpixel( mergedBitmap, xOffset + sprite->getX(), yOffset + sprite->getY(), makecol(255,0,0));
               putpixel( mergedBitmap, xOffset + sprite->getX() + sprite->getWidth(), yOffset + sprite->getY(), makecol(255,0,0));
               putpixel( mergedBitmap, xOffset + sprite->getX() + sprite->getWidth(), yOffset + sprite->getY() + sprite->getHeight(), makecol(255,0,0));
               putpixel( mergedBitmap, xOffset + sprite->getX(), yOffset + sprite->getY() + sprite->getHeight(), makecol(255,0,0));
            }
         }
      }
      
      for (int j = 0 ; j < objects.size() ; j++)
      {
         if (objects[j] != NULL)
         {
            std::vector<Point> boundaries = objects[j]->getBoundaryVector();

            for (int i = 0 ; i < boundaries.size() ; i++)
            {
               putpixel( mergedBitmap, xOffset + objects[j]->getX() + boundaries[i].x, yOffset + objects[j]->getY() + boundaries[i].y, makecol(0,0,255));
            }
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void DisplayManager::drawMenu(BITMAP* mergedBitmap,  // Populated in this call
                              std::vector<std::string> menuNames,
                              std::vector<std::string> menuItems,
                              int selectedIndex,
                              int xStartPixel)
{
   // (U) Determine longest string and thus the width of the recolor area.
   int maxStringSize = 0;
   
   for (int i = 0 ; i < menuItems.size() ;i++)
   {
      if (menuItems[i].size() > maxStringSize)
      {
         maxStringSize = menuItems[i].size();
      }
   }   

   int width = maxStringSize * 8 + 4;
   int breakHeight = 12 + (static_cast<int>(timeOnMenuAnimation / animationRunTimeInMilliseconds * (menuItems.size() * 10)));
   
   bool brokeOut = false;
   
   // (U) Recolor the area
   for (int i = 0 ; i < menuItems.size() ; i++)
   {
      for (int k = 0 ; k < 10 ; k++)
      {
         for (int j = 0 ; j < width ; j++)
         {
            if (13 + i * 10 + k > breakHeight)
            {
               brokeOut = true;
               break;
            }
            
            int color = getpixel(mergedBitmap, xStartPixel + j, 13 + i * 10 + k);
            
            int red = getr32(color);
            int green = getg32(color);
            int blue = getb32(color);
   
            if (i == selectedIndex)
            {         
               putpixel(mergedBitmap, xStartPixel + j, 13 + i * 10 + k, makecol(red / 2, green / 2, blue / 2));
            }
            else
            {
               putpixel(mergedBitmap, xStartPixel + j, 13 + i * 10 + k, makecol(red / 4, green / 4, blue / 4));
            }
         }
         
         if (brokeOut)
         {
            brokeOut = true;
            break;
         }
      }
      
      if (brokeOut)
      {
         brokeOut = true;
         break;
      }
   }
   
   if (!brokeOut)
   {
      menuAnimationFinished = true;
   }
   
   // (U) Write the text
   if (menuAnimationFinished)
   {
      for (int i = 0 ; i < menuItems.size() ; i++)
      {
         textprintf_ex(mergedBitmap, font, xStartPixel + 2, 14 + i * 10, makecol(255, 255, 255), -1, "%s", menuItems[i].c_str());   
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void DisplayManager::displayTextWindow(BITMAP* mergedBitmap,
                                       std::string text,
                                       std::string characterName)

{
   int textAreaWidth = 400;
   int textStart = 0;
   int imageWidth = 50;

   if(characterName.length() > 0)
   {
      textAreaWidth = 400 - imageWidth;
      textStart = imageWidth;
   }

   int columnBuffer = 10;
   int textAreaColumns = (textAreaWidth - columnBuffer) / 8;

   std::vector<std::string> lines;
   
   std::stringstream ss;
   ss << text;
   
   std::string nextLine;
   
   while(!ss.fail())
   {
      std::string word;
      ss >> word;

      if (!ss.fail())
      {
         if (nextLine.size() + word.size() < textAreaColumns)
         {
            if (nextLine.compare("") == 0)
            {
               nextLine = word;
            }
            else
            {
               nextLine = nextLine + " " + word;
            }
         }
         else
         {
            lines.push_back(nextLine);
            nextLine = word;
         }
      }
      else
      {
         lines.push_back(nextLine);
      }
   }
   
   int rowBuffer = 2;
   int rows = lines.size() + options.size() + rowBuffer;

   if (characterName.length() > 0 && rows < rowBuffer + imageWidth / 10)
   {
      rows = rowBuffer + imageWidth / 10 - 1;
   }

   for (int y = ((screenHeight - (rows * 10)) / 2) ; y < (rows * 10) + ((screenHeight - (rows * 10)) / 2) ; y++)
   {
      for (int x = (screenWidth / 2) - ((textAreaWidth + textStart) / 2) ; x < (screenWidth / 2) + ((textAreaWidth + textStart) / 2) ; x++)
      {
         int color = getpixel(mergedBitmap, x, y);
         
         int red = getr32(color);
         int green = getg32(color);
         int blue = getb32(color);

         // (U) Light background
         putpixel(mergedBitmap, x, y, makecol(255 - red / 5, 255 - green / 5, 255 - blue / 5));
         
         // (U) Dark background
         //putpixel(mergedBitmap, x, y, makecol(red / 5, green / 5, blue / 5));
      }
   }

   if (characterName.length() > 0)
   {
      BITMAP* characterBitmap = load_bitmap((std::string("Character Images/") + characterName + std::string(".bmp")).c_str(), NULL);

      if (characterBitmap)
      {
         stretch_sprite(mergedBitmap, characterBitmap, (screenWidth / 2) - ((textAreaWidth + imageWidth) / 2), (screenHeight - ((rows - 1) * 10)) / 2, imageWidth, imageWidth);
      }

      destroy_bitmap(characterBitmap);
   }

   for (int i = 0 ; i < lines.size() ;i++)
   {
      // (U) Light font
      //textprintf_ex(mergedBitmap, font, ((screenWidth - textAreaWidth) / 2) + (columnBuffer / 2), ((screenHeight - ((rows - rowBuffer) * 10)) / 2) + i * 10 + 1, makecol(255, 255, 255), -1, "%s", lines[i].c_str());

      // (U) Dark font
      textprintf_ex(mergedBitmap, font, (screenWidth / 2) - ((textAreaWidth + textStart) / 2) + (columnBuffer / 2) + textStart, ((screenHeight - ((rows - rowBuffer) * 10)) / 2) + i * 10 + 1, makecol(0, 0, 0), -1, "%s", lines[i].c_str());
   }

   for (int i = 0 ; i < options.size() ;i++)
   {
      textprintf_ex(mergedBitmap, font, ((screenWidth - textAreaWidth) / 2) + (columnBuffer / 2), ((screenHeight - ((rows - rowBuffer) * 10)) / 2) + lines.size() * 10 + i * 10 + 1, makecol(0, 0, 0), -1, "%s%s", (selectedResult == i ? "> " : "  "), options[i].c_str());
   }
}

////////////////////////////////////////////////////////////////////////////////

void DisplayManager::handleUserInput(double dt)
{
   if (key[KEY_ALTGR] || key[KEY_ALT])
   {
      altDown = true;
   }
   // alt was down, it's not anymore, and it wasn't used, start displaying the
   // menu then.
   else if (altDown && !altUsed)
   {
      menuDisplayed = true;
      menuAnimationFinished = false;
      timeOnMenuAnimation = 0;
      selectedItem = 0;
      selectedMenu = 0;

      altDown = false;
      altUsed = false;
   }

   // (U) Handle user input
   while (keypressed())
   {
      int nextKey = readkey();
      //int asciiValue = (nextKey & 0xff);
      int scanCode = (nextKey >> 8);
      //int ctrlLetter = (nextKey & 0xff);
      //int altLetter = (nextKey >> 8);

      if (!textWindowActive && !menuDisplayed && !displayInventory && !loadFileWindowActive && !saveFileWindowActive && !dialogWindowActive)
      {
         if(!(key_shifts & KB_CTRL_FLAG) && !(key_shifts & KB_ALT_FLAG))
         {
            // (U) Backspace / delete (mac)
            if (nextKey == 16136 || nextKey == 16255)
            {
               if (currentCommand.size() > 0)
               {
                 currentCommand = currentCommand.substr(0, currentCommand.size() - 1);
               }

               keyDown = true;
            }
            // (U) Enter
            else if(nextKey == 17165)
            {
               // (U) Process the new command from the user - this will loop through
               // all the text input and will compare the current command with
               // the text in the criteria
               sceneManager.processInput();
               clear_keybuf();

               currentCommand = "";

               keyDown = true;
            }
            // (U) Check that it is a valid, text character
            else if (nextKey < 12000 || nextKey == 19232)
            {
               if (currentCommand.size() < 77)
               {
                  currentCommand.push_back(static_cast<char>(nextKey));
               }

               keyDown = true;
            }

            lastChar = nextKey;
         }
      }
      
      if (!keyDown)
      {
         keyDown = true;

         // (U) Ctrl combinations
         if(key_shifts & KB_CTRL_FLAG)
         {
            if (scanCode == KEY_EQUALS)
            {
               if (speed < 2)
               {
                  speed = speed * 2.0;
               }
               else
               {
                  speed = 2.0;
               }
            }
            else if (scanCode == KEY_MINUS)
            {
               if (speed > 0.5)
               {
                  speed = speed / 2.0;
               }
               else
               {
                  speed = 0.5;
               }
            }
            else if(scanCode == KEY_O || scanCode == KEY_L)
            {
               loadFileWindowActive = true;
               fileNames = getdir("Saved Games/",".svg");
            }
            else if(scanCode == KEY_S)
            {
               saveFileWindowActive = true;
               fileNames = getdir("Saved Games/",".svg");
            }
         }
         // (U) Alt
         else if (key_shifts & KB_ALT_FLAG)
         {
            if (scanCode == KEY_F4)
            {
               altUsed = true;
               wantToQuit = true;
            }
            else if (scanCode == KEY_ENTER)
            {
               altUsed = true;
               displayDriver.toggleFullScreen();
            }
         }
         // (U) Enter gets us out of text window active mode
         else if (textWindowActive)
         {
            if (scanCode == KEY_ENTER)
            {
               textSequenceNumber++;

               if (textSequenceNumber == textToDisplay.size())
               {
                  textWindowActive = false;
               }
            }
         }
         else if(dialogWindowActive)
         {
            if (scanCode == KEY_ENTER)
            {
               dialogWindowActive = false;

               switch (dialogType)
               {
                  case NewGame:
                  {
                     if (selectedResult == 0)
                     {
                        loadFile("Start.psg");
                     }

                     break;
                  }
                  case GameOver:
                  {
                     if (selectedResult == 0)
                     {
                        loadFile("Start.psg");
                     }
                     else if (selectedResult == 1)
                     {
                        loadFileWindowActive = true;
                        fileNames = getdir("Saved Games/",".svg");
                     }
                     else if (selectedResult == 2)
                     {
                        wantToQuit = true;
                     }

                     break;
                  }
               }
            }
            else if(scanCode == KEY_UP)
            {
               if (selectedResult > 0)
               {
                  selectedResult--;
               }
            }
            else if(scanCode == KEY_DOWN)
            {
               if (selectedResult < options.size() - 1)
               {
                  selectedResult++;
               }
            }
         }

         // (U) Enter selects the current menu and closes it
         else if(menuDisplayed && scanCode == KEY_ENTER)
         {
            menuItemSelected(menus[selectedMenu][selectedItem]);

            menuDisplayed = false;
            menuAnimationFinished = true;
         }

         // (U) Enter selects the current file and loads it
         else if(loadFileWindowActive || saveFileWindowActive)
         {
            if (scanCode == KEY_ENTER)
            {
               if (loadFileWindowActive && selectedLoadFile < fileNames.size())
               {
                  loadFile(fileNames[selectedLoadFile]);
               }
               else if(saveFileWindowActive && selectedSaveFile < fileNames.size() + 1)
               {
                  if (selectedSaveFile == fileNames.size() && newFileName.length() > 0)
                  {
                     saveFile(newFileName + ".svg");
                     newFileName = "";
                  }
                  else if(selectedSaveFile < fileNames.size())
                  {
                     saveFile(fileNames[selectedSaveFile]);
                  }
               }

               loadFileWindowActive = false;
               saveFileWindowActive = false;
            }
            // (U) Esc gets us out of the current menu
            else if (scanCode == KEY_ESC)
            {
               loadFileWindowActive = false;
               saveFileWindowActive = false;
               newFileName = "";
            }
            else if(scanCode == KEY_UP)
            {
               if (loadFileWindowActive && selectedLoadFile > 0)
               {
                  selectedLoadFile--;
               }
               else if(saveFileWindowActive && selectedSaveFile > 0)
               {
                  selectedSaveFile--;
               }
            }
            else if(scanCode == KEY_DOWN)
            {
               if (saveFileWindowActive && selectedSaveFile < fileNames.size())
               {
                  selectedSaveFile++;
               }
               else if(loadFileWindowActive && selectedLoadFile < fileNames.size() - 1)
               {
                  selectedLoadFile++;
               }
            }
            else if (saveFileWindowActive)
            {
               if (selectedSaveFile == fileNames.size())
               {
                  // (U) Backspace
                  if (nextKey == 16136)
                  {
                     if (newFileName.size() > 0)
                     {
                       newFileName = newFileName.substr(0, newFileName.size() - 1);
                     }
                  }
                  // (U) Check that is a valid character
                  else if (nextKey < 12000 || nextKey == 19232)
                  {
                     newFileName.push_back(static_cast<char>(nextKey));
                  }
               }
            }
         }
         else if(menuDisplayed)
         {
            if (scanCode == KEY_ESC)
            {
               menuDisplayed = false;
               menuAnimationFinished = true;
            }
            else if(scanCode == KEY_DOWN)
            {
               if (menuDisplayed)
               {
                  selectedItem++;

                  if (selectedMenu < menus.size() && selectedItem >= menus[selectedMenu].size())
                  {
                     selectedItem = menus[selectedMenu].size() - 1;
                  }
               }
            }
            else if(scanCode == KEY_UP)
            {
               if (menuDisplayed)
               {
                  selectedItem--;

                  if (selectedItem < 0)
                  {
                     selectedItem = 0;
                  }
               }
            }
            else if (scanCode == KEY_RIGHT)
            {
               selectedMenu++;

               if (selectedMenu >= menus.size())
               {
                  selectedMenu = menus.size() - 1;
               }
               else
               {
                  selectedItem = 0;
               }

               menuAnimationFinished = false;
               timeOnMenuAnimation = 0;
            }
            else if(scanCode == KEY_LEFT)
            {
               selectedMenu--;

               if (selectedMenu < 0)
               {
                  selectedMenu = 0;
               }

               menuAnimationFinished = false;
               timeOnMenuAnimation = 0;
            }
         }
         else if(displayInventory)
         {
            if (scanCode == KEY_RIGHT)
            {
               inventoryMovementSpeed += inventoryMaxMovementSpeedPps * -1;
            }
            else if(scanCode == KEY_LEFT)
            {
               inventoryMovementSpeed += inventoryMaxMovementSpeedPps;
            }
            else if(scanCode == KEY_TAB || scanCode == KEY_ENTER)
            {
               displayInventory = false;
            }
         }
         else if (scanCode == KEY_UP || scanCode == KEY_DOWN || scanCode == KEY_LEFT || scanCode == KEY_RIGHT)
         {
            characterDirection = scanCode;
         }
         else if (scanCode == KEY_TAB)
         {
            inventoryDisplayX = 0;
            inventoryMovementSpeed = 0;
            displayInventory = true;
         }
         else if (scanCode == KEY_F11)
         {
            audioManager.toggleMusic();
         }
         else if (scanCode == KEY_EQUALS)
         {
            speed = 1.0;
         }
         else if(scanCode == KEY_F1)
         {
            debugXYOn = !debugXYOn;
         }
         else if(scanCode == KEY_F2)
         {
            debugVsyncOff = !debugVsyncOff;
         }
         else if(scanCode == KEY_F3)
         {
            debugRestOff = !debugRestOff;
         }
         else if(scanCode == KEY_F4)
         {
           debugBordersOn = !debugBordersOn;
         }
         else if(scanCode == KEY_F5)
         {
            debugFpsOn = !debugFpsOn;
         }
         else if(scanCode == KEY_F12)
         {
            debugXYOn = !debugXYOn;
            debugVsyncOff = !debugVsyncOff;
            debugRestOff = !debugRestOff;
            debugBordersOn = !debugBordersOn;
            debugFpsOn = !debugFpsOn;
         }
      }
   }

   if (!keypressed() && keyDown)
   {
      keyDown = false;
      altDown = false;
      altUsed = false;
   }
}

////////////////////////////////////////////////////////////////////////////////

void DisplayManager::menuItemSelected(std::string menuItemName)
{
   if(menuItemName.compare("Save ... Ctrl - S") == 0)
   {
      saveFileWindowActive = true;
      fileNames = getdir("Saved Games/",".svg");
   }
   else if(menuItemName.compare("Open ... Ctrl - O") == 0)
   {
      loadFileWindowActive = true;
      fileNames = getdir("Saved Games/",".svg");
   }
   else if (menuItemName.compare("New .... Ctrl - N") == 0)
   {
      std::vector<std::string> output;
      output.push_back("Are you sure you want to start over?");

      std::vector<std::string> menuOptions;
      menuOptions.push_back("Yup");
      menuOptions.push_back("No");

      displayText(output, std::vector<std::string>(), menuOptions, NewGame);
   }
   else if (menuItemName.compare("Exit") == 0)
   {
      wantToQuit = true;
   }
   // (U) Options menu
   else if(menuItemName.compare("Speed Up .............. Ctrl - =") == 0)
   {
      if (speed < 2)
      {
         speed = speed * 2.0;
      }
      else
      {
         speed = 2.0;
      }
   }
   else if(menuItemName.compare("Slow Down ............. Ctrl - -") == 0)
   {
      if (speed > 0.5)
      {
         speed = speed / 2.0;
      }
      else
      {
         speed = 0.5;
      }
   }
   else if(menuItemName.compare("Toggle Sticky Keys") == 0)
   {
      useStickyKeys = !useStickyKeys;
   }
   else if (menuItemName.compare("Toggle Music .......... F11") == 0)
   {
      audioManager.toggleMusic();
   }
   else if(menuItemName.compare("Toggle Full Screen .... Alt - Enter") == 0)
   {
      displayDriver.toggleFullScreen();
   }
   // (U) Help menu
   else if(menuItemName.compare("Help") == 0)
   {
      std::vector<std::string> output;
      output.push_back("Your goal is to help Ernie solve the challenges in his adventure.  Start by \"looking\" in each scene and try \"talking\" to each character to meet.  You will have to \"get\" several objects throughout the game.  Hitting TAB will bring up the current inventory.  Alt will bring up the menu bar.");
      displayText(output, std::vector<std::string>());
   }
   else if(menuItemName.compare("About") == 0)
   {
      std::vector<std::string> output;
      output.push_back("Ernie's Adventure was designed and created by Andrew and Alex Laird.  We hope you enjoy solving the challenges of the game and helping Ernie get to the end of his adventure.");
      displayText(output, std::vector<std::string>());
   }
   else
   {
      std::cerr << "Unsupported Menu Item Selected" << std::endl;
   }
}

////////////////////////////////////////////////////////////////////////////////

void DisplayManager::loadFile(std::string fileName)
{
   std::ifstream savedFile;
   std::string filePath = "Saved Games/" + fileName;
   savedFile.open(filePath.c_str());
   
   if (savedFile.is_open())
   {
      // (U) We'll respawn all the characters that have been saved later
      spriteManager.resetSpawnedCharacters();

      std::string line;
   
      bool abort = false;
   
      while (!abort && !savedFile.eof())
      {
         std::getline(savedFile, line);
         
         if(line.compare("SAVED_VERSION") == 0)
         {
            std::getline(savedFile, line);
            
            std::cerr << std::endl << "Loading Version of the Saved Game File" << std::endl;
            
            std::stringstream ss;
            ss << line;
            double savedVersion;
            ss >> savedVersion;
            std::cerr << "Version: " << savedVersion << std::endl;
            
            if (!ss.fail() && savedVersion != version)
            {
               std::cerr << std::endl << "The saved game was from a previous version and can't be loaded" << std::endl;

               std::vector<std::string> output;
               output.push_back("The saved game was from a previous version and can't be loaded.");

               displayText(output, std::vector<std::string>());
               abort = true;
            }
            else if(ss.fail())
            {
               abort = true;
            }
         }
         else if(line.compare("CURRENT_SCENE") == 0)
         {
            std::getline(savedFile, line);
            
            std::cerr << std::endl << "Loading Current Scene from the Saved Game File" << std::endl;
            
            std::stringstream ss;
            ss << line;
            std::string sceneEnumString;
            ss >> sceneEnumString;
            std::cerr << "Current Scene: " << sceneEnumString << std::endl;
            
            if (!ss.fail())
            {
               sceneManager.changeScene(sceneManager.sceneNameToSceneId(sceneEnumString));
            }
            else if(ss.fail())
            {
               abort = true;
            }
         }
         else if(line.compare("GAME_IS_FINISHED") == 0)
         {
            std::getline(savedFile, line);
            
            std::cerr << std::endl << "Loading Game Is Finished from the Saved Game File" << std::endl;
            
            std::stringstream ss;
            ss << line;
            std::string gameIsFinishedString;
            ss >> gameIsFinishedString;
            std::cerr << "Game is finished: " << gameIsFinishedString << std::endl;
            
            if (!ss.fail())
            {
               gameFinished = StringToBool(gameIsFinishedString);
            }
            else if(ss.fail())
            {
               abort = true;
            }
         }
         else if(line.compare("UI_DISABLED") == 0)
         {
            std::getline(savedFile, line);

            std::stringstream ss;
            ss << line;
            std::string uiDisabledString;
            ss >> uiDisabledString;
            std::cerr << "UI Disabled: " << uiDisabledString << std::endl;

            if (!ss.fail())
            {
               uiDisabled = StringToBool(uiDisabledString);
            }
            else
            {
               abort = true;
            }
         }
         else if(line.compare("ELAPSED_TIME") == 0)
         {
            std::getline(savedFile, line);

            std::stringstream ss;
            ss << line;
            double tempElapsedTime;
            ss >> tempElapsedTime;
            std::cerr << "Elapsed Time: " << tempElapsedTime << std::endl;

            if (!ss.fail())
            {
               elapsedTime = tempElapsedTime;
            }
            else
            {
               abort = true;
            }
         }
         else if (line.compare("EVENTS") == 0)
         {
            std::cerr << "Loading Events" << std::endl;

            do
            {
               std::getline(savedFile, line);

               std::stringstream ss;
               ss << line;

               std::string eventId;
               std::string hasExecutedString;

               ss >> eventId;
               ss >> hasExecutedString;

               std::cerr << "Loading Event - Id: " << eventId << " Has Executed: " << hasExecutedString << std::endl;

               bool hasExecuted = StringToBool(hasExecutedString);

               if (!ss.fail())
               {
                  sceneManager.setEventHasExecuted(eventId, hasExecuted);
               }
            }
            while(line.compare("") != 0);
         }
         else if(line.compare("INTERNAL_VARIABLES") == 0)
         {
            std::cerr << "Loading Internal Variables" << std::endl;

            std::getline(savedFile, line);
            
            std::stringstream ss;
            ss << line;
            unsigned short score;
            ss >> score;
            
            if (!ss.fail())
            {
               std::cerr << "Loading Score: " << score << std::endl;

               variableManager.resetScore();
               variableManager.addToScore(score);

               do
               {
                  std::getline(savedFile, line);
               
                  std::stringstream ss;
                  ss << line;
                  std::string variableName;
                  unsigned short value;
                  
                  ss >> variableName;
                  ss >> value;
                  
                  if (!ss.fail())
                  {
                     std::cerr << "Loading Internal Variable - Name: " << variableName << " Value: " << value << std::endl;
                     
                     variableManager.updateVariable(variableName, value);
                  }
               }
               while(line.compare("") != 0);
            }
            else
            {
               abort = true;
            }
         }
         else if(line.compare("INVENTORY_OBJECTS") == 0)
         {
            std::cerr << "Loading Inventory Objects" << std::endl;

            do
            {
               std::getline(savedFile, line);
            
               std::stringstream ss;
               ss << line;
               std::vector<std::string> inventoryObjectWords;

               std::string nextInvWord;

               ss >> nextInvWord;

               while (!ss.fail())
               {
                  inventoryObjectWords.push_back(nextInvWord);
                  ss >> nextInvWord;
               }

               if (inventoryObjectWords.size() >= 2)
               {
                  std::string inventoryObjectString = "";

                  for (int i = 0 ; i < inventoryObjectWords.size() - 1 ; i++)
                  {
                     if (i == 0)
                     {
                        inventoryObjectString = inventoryObjectWords[i];
                     }
                     else
                     {
                        inventoryObjectString = inventoryObjectString + " " + inventoryObjectWords[i];
                     }
                  }

                  std::cerr << "Loading Inventory Object - Name: " << inventoryObjectString << " IsInInventory: " << StringToBool(inventoryObjectWords[inventoryObjectWords.size() - 1]) << std::endl;
                  
                  inventory.setInInventory(inventory.inventoryItemNameToId(inventoryObjectString), StringToBool(inventoryObjectWords[inventoryObjectWords.size() - 1]));
               }
            }
            while(line.compare("") != 0);
         }
         else if(line.compare("CHARACTERS") == 0)
         {
            std::cerr << "Loading Characters" << std::endl;
            
            do
            {
               std::getline(savedFile, line);
            
               std::stringstream ss;
               ss << line;

               std::string sceneName;
               std::string guiId;
               std::string isMainCharacterString;
               int x;
               int y;
               std::string inGameString;
               double pps;
               std::string onScreenString;
               int currentFrame;
               double timeOnFrame;

               ss >> sceneName;
               ss >> guiId;
               ss >> isMainCharacterString;
               ss >> x;
               ss >> y;
               ss >> inGameString;
               ss >> pps;
               ss >> onScreenString;
               ss >> currentFrame;
               ss >> timeOnFrame;

               bool isMainCharacter = StringToBool(isMainCharacterString);
               bool inGame = StringToBool(inGameString);
               bool onScreen = StringToBool(onScreenString);

               int dashLocation = guiId.find("-");

               if (dashLocation != -1)
               {
                  std::string spawnerId = guiId.substr(0, dashLocation);

                  // (U) This was a spawned character and has been removed from
                  // the game if it existed, spawn the character now.
                  Character* newCharacter = spriteManager.spawnCharacter(spawnerId, x, y, false, sceneManager.sceneNameToSceneId(sceneName));

                  if (newCharacter != NULL)
                  {
                     spriteManager.loadCharacterSettings(newCharacter->getGuiId(),
                             isMainCharacter,
                             x,
                             y,
                             inGame,
                             pps,
                             onScreen,
                             currentFrame,
                             timeOnFrame);
                  }
               }
               else
               {
                  if (!ss.fail())
                  {
                     std::cerr << "Loading Charater - Id: " << guiId << " IsMainCharacter: " << isMainCharacter << " X: " << x << " Y: " << y << " InGame: " << inGame << " pps: " << pps << " onscreen: " << onScreen << std::endl;

                     std::string extraStepsType = "";

                     ss >> extraStepsType;

                     if (!ss.fail())
                     {
                        if (extraStepsType.compare("Path") == 0)
                        {
                           int currentActiveCommandInt;
                           int stepNumber;
                           int commandInStep;
                           std::string waitForNextStepString;
                           std::string activeSpriteName;
                           double timeInWait;
                           double totalTimeToWaitFor;
                           double moveXYRatio;
                           short moveToLocationX;
                           short moveToLocationY;
                           double timeInMove;
                           double timeOnMoveSoFar;
                           std::string ignoreBordersString;
                           short startOfStepPointX;
                           short startOfStepPointY;

                           ss >> currentActiveCommandInt;
                           ss >> stepNumber;
                           ss >> commandInStep;
                           ss >> waitForNextStepString;
                           ss >> activeSpriteName;
                           ss >> timeInWait;
                           ss >> totalTimeToWaitFor;
                           ss >> moveXYRatio;
                           ss >> moveToLocationX;
                           ss >> moveToLocationY;
                           ss >> timeInMove;
                           ss >> timeOnMoveSoFar;
                           ss >> ignoreBordersString;
                           ss >> startOfStepPointX;
                           ss >> startOfStepPointY;

                           if (!ss.fail())
                           {
                              std::cerr << "Loading Path Charater - stepNumber: " << stepNumber << " commandInStep: " << isMainCharacter << " waitForNextStepString: " << waitForNextStepString << " timeInWait: " << timeInWait << " startOfStepPointX: " << startOfStepPointX << " startOfStepPointY: " << startOfStepPointY << std::endl;
                              spriteManager.loadCharacterSettings(guiId,
                                      isMainCharacter,
                                      x,
                                      y,
                                      inGame,
                                      pps,
                                      onScreen,
                                      static_cast<PathCommand>(currentActiveCommandInt),
                                      stepNumber,
                                      commandInStep,
                                      StringToBool(waitForNextStepString),
                                      activeSpriteName,
                                      timeInWait,
                                      totalTimeToWaitFor,
                                      moveXYRatio,
                                      Point (moveToLocationX, moveToLocationY),
                                      timeInMove,
                                      timeOnMoveSoFar,
                                      StringToBool(ignoreBordersString),
                                      Point(startOfStepPointX, startOfStepPointY));
                           }
                        }
                        else if (extraStepsType.compare("Follow") == 0)
                        {
                           std::string followingString;

                           ss >> followingString;

                           if (!ss.fail())
                           {
                              std::cerr << "Loading Follow Charater - followingString: " << followingString << std::endl;

                              spriteManager.loadCharacterSettings(guiId,
                                      isMainCharacter,
                                      x,
                                      y,
                                      inGame,
                                      pps,
                                      onScreen,
                                      currentFrame,
                                      timeOnFrame,
                                      StringToBool(followingString));
                           }
                        }
                     }
                     else
                     {
                        spriteManager.loadCharacterSettings(guiId,
                                isMainCharacter,
                                x,
                                y,
                                inGame,
                                pps,
                                onScreen,
                                currentFrame,
                                timeOnFrame);
                     }
                  }
               }
            }
            while(line.compare("") != 0);
         }
      }
      
      savedFile.close();

      sceneManager.refreshCanMoveHereVector();
      sceneManager.refreshCanMoveHereVector();
   }
}

////////////////////////////////////////////////////////////////////////////////

void DisplayManager::saveFile(std::string fileName)
{
   std::cerr << std::endl << "SAVING FILE TO: " << fileName << std::endl;
   std::cerr << "SAVED_VERSION" << std::endl << version << std::endl << std::endl;
   std::cerr << "GAME_IS_FINISHED" << std::endl << BoolToString(gameFinished) << std::endl << std::endl;
   std::cerr << "UI_DISABLED" << std::endl << BoolToString(uiDisabled) << std::endl << std::endl;
   std::cerr << "ELAPSED_TIME" << std::endl << elapsedTime << std::endl << std::endl;
   std::cerr << sceneManager.getSaveString();
   std::cerr << spriteManager.getSaveString();
   
   std::ofstream savedFile;
   std::string filePath = "Saved Games/" + fileName;
   savedFile.open(filePath.c_str());
   
   if (savedFile.is_open())
   {
      savedFile << "SAVED_VERSION" << std::endl << version << std::endl << std::endl;
      savedFile << "GAME_IS_FINISHED" << std::endl << BoolToString(gameFinished) << std::endl << std::endl;
      savedFile << "UI_DISABLED" << std::endl << BoolToString(uiDisabled) << std::endl << std::endl;
      savedFile << "ELAPSED_TIME" << std::endl << elapsedTime << std::endl << std::endl;
      savedFile << sceneManager.getSaveString();
      savedFile << spriteManager.getSaveString();
   }
   
   savedFile.close();
}

////////////////////////////////////////////////////////////////////////////////

bool DisplayManager::loadSkySettings()
{
   bool status = true;

   std::cerr << std::endl << "Loading Sky Settings" << std::endl;

   std::string line;

   std::ifstream skySettingsFile("SkyChange.scf");
   if (skySettingsFile.is_open())
   {
      while (status && !skySettingsFile.eof())
      {
         std::getline(skySettingsFile, line);

         std::stringstream ss;
         ss << line;
         int red;
         int green;
         int blue;
         int seconds;

         ss >> red;
         ss >> green;
         ss >> blue;
         ss >> seconds;

         if (!ss.fail())
         {
            skyColors.push_back(SkyColor(red, green, blue, seconds));
         }
      }

      skySettingsFile.close();
   }
   else
   {
      status = false;
   }

   return status;
}
