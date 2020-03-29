#include "SpriteWindow.h"
#include "Sprite.h"
#include "SceneSettings.h"
#include "SceneWindow.h"
#include "GlobalTimer.h"

#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <math.h>

SpriteWindow::SpriteWindow(SceneWindow& inSceneWindow)
 : BaseWindow(),
   currentSpriteNames(),
   currentMode(Border),
   blueMouse(NULL),
   redMouse(NULL),
   greenMouse(NULL),
   currentSprite(NULL),
   lastMouseZ(mouse_z),
   leftMouseDown(false),
   rightMouseDown(false),
   writeOn(false),
   lastClickedBorderPoint(-1, -1),
   lastClickedLayerPoint(-1, -1),
   rightMouseClickDownPoint(-1, -1),
   leftMouseClickDownPoint(-1, -1),
   keyDown(false),
   shadowDrawOn(false),
   shadowBackground(true),
   currentMenuItem(0),
   direction(Up),
   currentGroundPixel(0),
   sceneWindow(inSceneWindow),
   lastXy(-1, -1),
   shadow(NULL)
{
   blueMouse = load_bitmap("SceneWriter2Code/Images/blueMouse.bmp", 0);
   redMouse = load_bitmap("SceneWriter2Code/Images/redMouse.bmp", 0);
   orangeMouse = load_bitmap("SceneWriter2Code/Images/orangeMouse.bmp", 0);
   greenMouse = load_bitmap("SceneWriter2Code/Images/greenMouse.bmp", 0);
   purpleMouse = load_bitmap("SceneWriter2Code/Images/purpleMouse.bmp", 0);

   if (blueMouse == 0 ||
       redMouse == 0 ||
       orangeMouse == 0 ||
       greenMouse == 0 ||
       purpleMouse == 0)
   {
      // Shut down Allegro
      allegro_exit();    
       
      std::cout << "Error creating mouse images for sprite window\n";
       
      system("PAUSE");
      exit(1);
   }
   
   setup_timer();
}

////////////////////////////////////////////////////////////////////////////////

SpriteWindow::~SpriteWindow()
{
   destroy_bitmap(shadow);
}

////////////////////////////////////////////////////////////////////////////////

void SpriteWindow::getNextFrame(BITMAP* frame)
{
   rectfill(frame, 0, 0, 640, 480, makecol(255,200,255));
   
   for (int i = 0 ; i < 3 ; i++)
   {
      for (int j = 0 ; j < 10 ; j++)
      {
         textprintf_ex(frame, font, j * 10 + i * 220, j * 50, makecol(200, 200, 200), -1, "Sprite Window");         
      }
   }
   
   if (currentSprite != NULL)
   {
      int x = 320 - currentSprite->getWidth() / 2;
      int y = 240 - currentSprite->getHeight() / 2;

      drawGrid(frame, display_dt);

      if (shadowBackground)
      {
         draw_sprite(frame, shadow, 0, 0);
      }

      draw_sprite(frame, currentSprite->getCurrentImage(), x, y);      

      std::vector<Point> boundaries = currentSprite->getBoundaryVector();
   
      // (U) Boundaries
      for (int i = 0 ; i < boundaries.size() ; i++)
      {
         putpixel( frame, x + boundaries[i].x, y + boundaries[i].y, makecol(0,0,255));
      }
      
      // (U) Heights
      std::vector<short> heights = currentSprite->getHeightVector();
      
      for (int i = 0 ; i < heights.size() ; i++)
      {
         if (heights[i] != -1)
         {
            putpixel( frame, x + i, y + heights[i], makecol(255,0,0));
         }
      }

      // (U) Boundary square points
      Square boundarySquare = currentSprite->getBoundarySquare();

      if (boundarySquare.upperLeft.x != -1)
      {
         putpixel(frame, x + boundarySquare.upperLeft.x, y + boundarySquare.upperLeft.y, makecol(63, 151, 4));
         putpixel(frame, x + boundarySquare.lowerRight.x, y + boundarySquare.lowerRight.y, makecol(63, 151, 4));
         putpixel(frame, x + boundarySquare.lowerRight.x, y + boundarySquare.upperLeft.y, makecol(63, 151, 4));
         putpixel(frame, x + boundarySquare.upperLeft.x, y + boundarySquare.lowerRight.y, makecol(63, 151, 4));
      }
      
      textprintf_ex(frame, font, 5, 5, makecol(255, 0, 0), -1, "%s Pixels Per Second: %0.2f", (currentMenuItem == 0 ? ">" : " "), currentSprite->getFrameRateChange());         
      
      std::vector<ImageStruct> imageStructs = currentSprite->getImageStructs();
      
      for (int i = 0 ; i < imageStructs.size() ;i++)
      {
         textprintf_ex(frame, font, 5, 15 + i *  10, makecol(255, 0, 0), -1, "%s %s %d", (currentMenuItem == i + 1 ? ">" : " "), imageStructs[i].imageName.c_str(), imageStructs[i].duration);   
      }

      Point referencePoint = currentSprite->getReferencePoint();

      if (referencePoint.x != -1 && referencePoint.y != -1)
      {
         int x = 320 - currentSprite->getWidth() / 2;
         int y = 240 - currentSprite->getHeight() / 2;

         line(frame, x + referencePoint.x - 2, y + referencePoint.y - 2, x + referencePoint.x + 2, y + referencePoint.y + 2, makecol(255, 128, 12));
         line(frame, x + referencePoint.x - 2, y + referencePoint.y + 2, x + referencePoint.x + 2, y + referencePoint.y - 2, makecol(255, 128, 12));
      }

      if (!shadowBackground)
      {
         draw_sprite(frame, shadow, 0, 0);
      }
   }

   if (lastClickedBorderPoint.x != -1 && lastClickedBorderPoint.y != -1)
   {
      line(frame, lastClickedBorderPoint.x, lastClickedBorderPoint.y, mouse_x, mouse_y, makecol(0, 0, 255));
   }
   
   if (lastClickedLayerPoint.x != -1 && lastClickedLayerPoint.y != -1)
   {
      line(frame, lastClickedLayerPoint.x, lastClickedLayerPoint.y, mouse_x, mouse_y, makecol(255, 0, 0));
   }
   
   if (rightMouseDown)
   {
      rect(frame, rightMouseClickDownPoint.x, rightMouseClickDownPoint.y, mouse_x, mouse_y, makecol(128, 128, 128));
   }

   switch(currentMode)
   {
      case Border:
      {
         draw_sprite(frame, blueMouse, mouse_x, mouse_y);      
         break;
      }
      case Layer:
      {
         draw_sprite(frame, redMouse, mouse_x, mouse_y);      
         break;
      }
      case Reference:
      {
         draw_sprite(frame, orangeMouse, mouse_x, mouse_y);
         break;
      }
      case BoundarySquare:
      {
         if (leftMouseDown)
         {
            rect(frame, leftMouseClickDownPoint.x, leftMouseClickDownPoint.y, mouse_x, mouse_y, makecol(63, 151, 4));
         }

         draw_sprite(frame, greenMouse, mouse_x, mouse_y);         
         break;
      }
      case Shadow:
      {
         if (shadowDrawOn)
         {
            if (key[KEY_L])
            {
               line(shadow, lastXy.x, lastXy.y, mouse_x, mouse_y, makecol(0, 0, 0));
               lastXy = Point(mouse_x, mouse_y);
            }
            else
            {
               clear_keybuf();
               std::cout << "shift down" << std::endl;
            }
         }

         draw_sprite(frame, purpleMouse, mouse_x, mouse_y);

         break;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
      
void SpriteWindow::process()
{
   if (mouse_z > lastMouseZ)
   {
      switch(currentMode)
      {
         case Border:
         {
            currentMode = Layer;
            break;
         }
         case Layer:
         {
            currentMode = Reference;
            break;
         }
         case Reference:
         {
            currentMode = BoundarySquare;
            break;
         }
         case BoundarySquare:
         {
            currentMode = Shadow;
            break;
         }
         case Shadow:
         {
            currentMode = Border;
            break;
         }
      }
      
      writeOn = false;
      lastMouseZ = mouse_z;
   }
   else if (mouse_z < lastMouseZ)
   {
      switch(currentMode)
      {
         case Border:
         {
            currentMode = Shadow;
            break;
         }
         case Layer:
         {
            currentMode = Border;
            break;
         }
         case Reference:
         {
            currentMode = Layer;
            break;
         }
         case BoundarySquare:
         {
            currentMode = Reference;
            break;
         }
         case Shadow:
         {
            currentMode = BoundarySquare;
            break;
         }
      }
      
      writeOn = false;
      lastMouseZ = mouse_z;
   }
   
   if (!leftMouseDown && (mouse_b & 1))
   {
      leftMouseDown = true;

      leftMouseClickDownPoint = Point(mouse_x, mouse_y);
   }
   else if (!rightMouseDown && (mouse_b & 2))
   {
      rightMouseDown = true;
      
      rightMouseClickDownPoint = Point(mouse_x, mouse_y);
   }
   else if (leftMouseDown || rightMouseDown)
   {
      if (leftMouseDown && !(mouse_b & 1))
      {
         leftMouseDown = false;
      
         switch(currentMode)
         {
            case Border:
            {
               mouseClickBorder();
               break;
            }
            case Layer:
            {
               mouseClickLayer();
               break;
            }
            case Reference:
            {
               mouseClickReference();
               break;
            }
            case BoundarySquare:
            {
               mouseClickBoundarySquare();
               break;
            }
            case Shadow:
            {
               mouseClickShadow();
               break;
            }
         }
      }
      else if(rightMouseDown && !(mouse_b & 2))
      {
         rightMouseDown = false;
         
         switch(currentMode)
         {
            case Border:
            {
               rightMouseClickBorder();
               break;
            }
            case Layer:
            {
               rightMouseClickLayer();
               break;
            }
            case Shadow:
            {
               rightClickShadow();
               break;
            }
         }
      }
   }
   
   checkKeyboard();
   
   
   // (U) Timing
   display_dt = display_elapsed_time - display_last_time;
   display_last_time = display_elapsed_time;

   // (U) Convert to milliseconds
   if (currentSprite != NULL)
   {
      currentSprite->timeChange(display_dt * 1000);
   }
}

////////////////////////////////////////////////////////////////////////////////

void SpriteWindow::mouseClickBorder()
{
   if (!writeOn)
   {
      writeOn = true;
      lastClickedBorderPoint = Point(mouse_x, mouse_y);
   }
   else
   {
      // (U) Save line
      std::vector<Point> points = lineToPoints(lastClickedBorderPoint.x, lastClickedBorderPoint.y, mouse_x, mouse_y);
      
      int x = 320 - currentSprite->getWidth() / 2;
      int y = 240 - currentSprite->getHeight() / 2;
      
      for(int i = 0 ; i < points.size() ; i++)
      {
         if (currentSprite != NULL && points[i].x - x < currentSprite->getWidth() && points[i].y - y < currentSprite->getHeight())
         {
            currentSprite->addBoundaryPoint(Point(points[i].x - x, points[i].y - y));
         }
      }
      
      if (key[KEY_LSHIFT] || key[KEY_RSHIFT])
      {
         lastClickedBorderPoint = Point(mouse_x, mouse_y);
      }
      else
      {
         writeOn = false;
         lastClickedBorderPoint = Point(-1, -1);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SpriteWindow::mouseClickLayer()
{
   if (!writeOn)
   {
      writeOn = true;
      lastClickedLayerPoint = Point(mouse_x, mouse_y);
   }
   else
   {
      // (U) Save line
      std::vector<Point> points = lineToPoints(lastClickedLayerPoint.x, lastClickedLayerPoint.y, mouse_x, mouse_y);
      
      int x = 320 - currentSprite->getWidth() / 2;
      int y = 240 - currentSprite->getHeight() / 2;
      
      for(int i = 0 ; i < points.size() ; i++)
      {
         if (currentSprite != NULL && points[i].x - x < currentSprite->getWidth() && points[i].x - x >= 0)
         {
            currentSprite->addHeightPoint(Point(points[i].x - x, points[i].y - y));
         }
      }
      
      if (key[KEY_LSHIFT] || key[KEY_RSHIFT])
      {
         lastClickedLayerPoint = Point(mouse_x, mouse_y);
      }
      else
      {
         writeOn = false;
         lastClickedLayerPoint = Point(-1, -1);
      }

      std::cout << currentSprite->getHeightVector().size() << std::endl;
   }
}

////////////////////////////////////////////////////////////////////////////////

void SpriteWindow::mouseClickReference()
{
   if (currentSprite != NULL)
   {
      int x = 320 - currentSprite->getWidth() / 2;
      int y = 240 - currentSprite->getHeight() / 2;

      currentSprite->setReferencePoint(Point(mouse_x - x, mouse_y - y));
   }
}

////////////////////////////////////////////////////////////////////////////////

void SpriteWindow::mouseClickBoundarySquare()
{
   if (currentSprite != NULL)
   {
      int x = 320 - currentSprite->getWidth() / 2;
      int y = 240 - currentSprite->getHeight() / 2;

      // (U) x1, y1 is upper left
      int x1 = -1;
      int x2 = -1;
      // (U) x2, y2 is lower right
      int y1 = -1;
      int y2 = -1;

      if (leftMouseClickDownPoint.x < mouse_x)
      {
         x1 = leftMouseClickDownPoint.x - x;
         x2 = mouse_x - x;
      }
      else
      {
         x1 = mouse_x - x;
         x2 = leftMouseClickDownPoint.x - x;
      }

      if (leftMouseClickDownPoint.y < mouse_y)
      {
         y1 = leftMouseClickDownPoint.y - y;
         y2 = mouse_y - y;
      }
      else
      {
         y1 = mouse_y - y;
         y2 = leftMouseClickDownPoint.y - y;
      }

      Point upperLeft(x1, y1);
      Point lowerRight(x2, y2);

      Square boundarySquare(upperLeft, lowerRight);

      currentSprite->setBoundarySquare(boundarySquare);
   }
}

////////////////////////////////////////////////////////////////////////////////


void SpriteWindow::mouseClickShadow()
{
   shadowDrawOn = !shadowDrawOn;

   if (shadowDrawOn)
   {
      lastXy = Point(mouse_x, mouse_y);
   }
}

////////////////////////////////////////////////////////////////////////////////

void SpriteWindow::rightMouseClickBorder()
{
   int xMin, xMax, yMin, yMax;
   
   if (rightMouseClickDownPoint.x < mouse_x)
   {
      xMin = rightMouseClickDownPoint.x;
      xMax = mouse_x;
   }
   else
   {
      xMax = rightMouseClickDownPoint.x;
      xMin = mouse_x;
   }
   
   if (rightMouseClickDownPoint.y < mouse_y)
   {
      yMin = rightMouseClickDownPoint.y;
      yMax = mouse_y;
   }
   else
   {
      yMax = rightMouseClickDownPoint.y;
      yMin = mouse_y;
   }
   
   int x = 320 - currentSprite->getWidth() / 2;
   int y = 240 - currentSprite->getHeight() / 2;
   
   for (int i = xMin ; i < xMax ; i++)
   {
      for (int j = yMin ; j < yMax ; j++)
      {
         if (currentSprite != NULL && i - x < currentSprite->getWidth() && j - y < currentSprite->getHeight())
         {
            currentSprite->clearBoundaryPoint(i - x, j - y);
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SpriteWindow::rightMouseClickLayer()
{
   int xMin, xMax;
   
   if (rightMouseClickDownPoint.x < mouse_x)
   {
      xMin = rightMouseClickDownPoint.x;
      xMax = mouse_x;
   }
   else
   {
      xMax = rightMouseClickDownPoint.x;
      xMin = mouse_x;
   }
      
   int x = 320 - currentSprite->getWidth() / 2;
      
   for (int i = xMin ; i < xMax ; i++)
   {
      if (currentSprite != NULL && i - x < currentSprite->getWidth())
      {
         currentSprite->clearHeightPoint(i - x);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SpriteWindow::rightClickShadow()
{
   floodfill(shadow, mouse_x, mouse_y, makecol(0, 0, 0));
}

////////////////////////////////////////////////////////////////////////////////

void SpriteWindow::checkKeyboard()
{
   if (key_shifts & KB_CTRL_FLAG)
   {
      // (U) Load sprite
      if (key[KEY_L])
      {
         if (!keyDown)
         {
            keyDown = true;
            
            int choice = userSelectFromMenu(makeObjectPlacementItems());
            
            if (choice != -1)
            {      
               std::string spriteName = currentSpriteNames[choice];
               
               std::string spriteFolderPath = sceneWindow.sceneManager.getSpriteManager().commonSpriteFolder + "/" + spriteName;
               std::string sptFilePath = spriteFolderPath + "/" + spriteName + ".spt";   
               std::string boundariesPath = spriteFolderPath + "/" + spriteName + ".bdr";
               
               struct stat stFileInfo;
               
               // (U) If it isn't in the common folder either, then check everywhere else
               if (stat(sptFilePath.c_str(),&stFileInfo) != 0 || stat(boundariesPath.c_str(),&stFileInfo) != 0)
               {
                  bool match = false;
                  
                  std::vector<std::string> sceneNames = sceneWindow.sceneManager.getSceneNames();
                  
                  for (int i = 0 ; !match && i < sceneNames.size() ; i++)
                  {
                     spriteFolderPath = sceneWindow.sceneManager.sceneFolder + "/" + sceneNames[i] + "/" + sceneWindow.sceneManager.getSpriteManager().spriteFolder + "/" + spriteName;
                     sptFilePath = spriteFolderPath + "/" + spriteName + ".spt";
                     boundariesPath = spriteFolderPath + "/" + spriteName + ".bdr";
            
                     // (U) Attempt to get the file attributes
                     // If the attributes can't be retrieved, then try the common folder
                     if (stat(sptFilePath.c_str(),&stFileInfo) == 0 && stat(boundariesPath.c_str(),&stFileInfo) == 0)
                     {
                        match = true;
                     }
                  }
               }

               std::cerr << "spriteFolderPath: " << spriteFolderPath << std::endl;                  
               std::cerr << "sptFilePath: " << sptFilePath << std::endl;
               std::cerr << "boundariesPath: " << boundariesPath << std::endl;
               
               currentSprite = new Sprite(sceneWindow.sceneManager.getSpriteManager(),
                                          spriteFolderPath,
                                          spriteName,
                                          mouse_x, 
                                          mouse_y, 
                                          1);
                                                          
               if (!currentSprite->loadSprite())
               {
                  std::cerr << "ERROR: Could not load sprite " << spriteName << std::endl;
                  delete currentSprite;
                  currentSprite = NULL;
                  
                  waitForAnyKey("ERROR: Could not load sprite " + spriteName);
               }
               else
               {
                  if (shadow)
                  {
                     destroy_bitmap(shadow);
                  }

                  if (currentSprite->getCurrentImage() != NULL)
                  {
                     shadow = create_bitmap(640, 480);
                     rectfill(shadow, 0, 0, shadow->w, shadow->h, makecol(255, 0, 255));
                  }

                  if (currentSprite->getShadowOffset().x != -1 && currentSprite->getShadowOffset().y != -1)
                  {
                     std::string shadowPath = currentSprite->getSpriteFolder() + "/" + currentSprite->getName() + "Shadow.bmp";

                     BITMAP* loadedShadow = load_bitmap(shadowPath.c_str(), NULL);

                     if (loadedShadow)
                     {
                        int spritex = 320 - currentSprite->getWidth() / 2;
                        int spritey = 240 - currentSprite->getHeight() / 2;

                        draw_sprite(shadow, loadedShadow, spritex + currentSprite->getShadowOffset().x, spritey + currentSprite->getShadowOffset().y);

                        destroy_bitmap(loadedShadow);
                     }
                  }

                  currentSprite->setSpriteRunning(true);
               }
            }
            // (U) Else esc
         }
      }
      // (U) Save scene object
      else if (key[KEY_S])
      {
         if (!keyDown)
         {
            keyDown = true;
            
            if (currentSprite != NULL)
            {
               //xorShadow();
               Point upperLeft(-1, -1);
               BITMAP* shadowToSave = cropShadow(upperLeft);

               int xOffset = -1;
               int yOffset = -1;

               if (shadowToSave && upperLeft.x != 0)
               {
                  int spritex = 320 - currentSprite->getWidth() / 2;
                  int spritey = 240 - currentSprite->getHeight() / 2;

                  xOffset = upperLeft.x - spritex;
                  yOffset = upperLeft.y - spritey;

                  std::string shadowPath = currentSprite->getSpriteFolder() + "/" + currentSprite->getName() + "Shadow.bmp";

                  PALETTE pal;
                  get_palette(pal);
                  save_bitmap(shadowPath.c_str(), shadowToSave, pal);

                  destroy_bitmap(shadowToSave);
               }

               currentSprite->saveSprite(xOffset, yOffset);
               //currentSprite->saveSceneBoundaryFile(xOffset, yOffset);
               sceneWindow.reloadSprite(currentSprite->getId());
            }
         }
      }
      else if (key[KEY_A])
      {
         if (!keyDown)
         {
            keyDown = true;
            
            if (currentSprite != NULL)
            {
               std::vector<ImageStruct> imageStructs = currentSprite->getImageStructs();
               
               std::string fileName = readString("Enter Image Name (No Extension)") + ".bmp";
               
               std::string filePath = currentSprite->getSpriteFolder()+ "/" + fileName;
               
               BITMAP* newBitmap = load_bitmap(filePath.c_str(), NULL);
               
               if (newBitmap == NULL)
               {
                  std::cerr << "ERROR: Could not load image for sprite - aborting: " << filePath << std::endl;
               }
               else
               {
                  ImageStruct imageStruct;
                  imageStruct.image = newBitmap;
                  imageStruct.imageName = fileName;
                  imageStruct.duration = 0;
                  
                  imageStructs.push_back(imageStruct);

                  currentSprite->setImageStructs(imageStructs);
               }
            }
         }
      }
      else if (key[KEY_PGDN])
      {
         if (currentSprite != NULL)
         {
            std::vector<ImageStruct> imageStructs = currentSprite->getImageStructs();
            
            for (int i = 0 ; i < imageStructs.size() ; i++)
            {
               imageStructs[i].duration -= 10;
            }
            
            currentSprite->setImageStructs(imageStructs);
         }
      }
      else if (key[KEY_PGUP])
      {
         if (currentSprite != NULL)
         {
            std::vector<ImageStruct> imageStructs = currentSprite->getImageStructs();
            
            for (int i = 0 ; i < imageStructs.size() ; i++)
            {
               imageStructs[i].duration += 10;
            }
            
            currentSprite->setImageStructs(imageStructs);
         }
      }
   }
   else if(key_shifts & KB_SHIFT_FLAG)
   {
      if (key[KEY_PGDN])
      {
         if (currentMenuItem == 0)
         {
            if (currentSprite != NULL && currentSprite->getFrameRateChange() > 0)
            {
               currentSprite->setFrameRateChange(currentSprite->getFrameRateChange() - 10);
            }
         }
         else
         {
            if (currentSprite != NULL)
            {
               std::vector<ImageStruct> imageStructs = currentSprite->getImageStructs();
               
               imageStructs[currentMenuItem - 1].duration -= 10;
               
               currentSprite->setImageStructs(imageStructs);
            }
         }
      }
      else if (key[KEY_PGUP])
      {
         if (currentMenuItem == 0)
         {
            if (currentSprite != NULL)
            {
               currentSprite->setFrameRateChange(currentSprite->getFrameRateChange() + 10);
            }
         }
         else
         {
            if (currentSprite != NULL)
            {
               std::vector<ImageStruct> imageStructs = currentSprite->getImageStructs();
               
               imageStructs[currentMenuItem - 1].duration += 10;
               
               currentSprite->setImageStructs(imageStructs);
            }
         }
      }
      else
      {
         keyDown = false;
      }
   }
   // (U) Not control key
   else
   {
      if (key[KEY_8_PAD])
      {
         if (!keyDown)
         {
            keyDown = true;
            
            direction = Up;
         }
      }
      else if (key[KEY_6_PAD])
      {
         if (!keyDown)
         {
            keyDown = true;
            
            direction = Right;
         }
      }
      else if (key[KEY_4_PAD])
      {
         if (!keyDown)
         {
            keyDown = true;
            
            direction = Left;
         }
      }
      else if (key[KEY_2_PAD])
      {
         if (!keyDown)
         {
            keyDown = true;
            
            direction = Down;
         }
      }
      else if (key[KEY_UP])
      {
         if (!keyDown)
         {
            keyDown = true;
            
            if (currentMenuItem > 0)
            {
               currentMenuItem--;
            }
         }
      }
      else if (key[KEY_DOWN])
      {
         if (!keyDown)
         {
            keyDown = true;
            
            currentMenuItem++;
         }
      }
      else if (key[KEY_PGDN])
      {
         if (!keyDown)
         {
            keyDown = true;

            if (currentMode == Shadow)
            {
               shadowBackground = !shadowBackground;
            }
            else
               {
               if (currentMenuItem == 0)
               {
                  if (currentSprite != NULL && currentSprite->getFrameRateChange() > 0)
                  {
                     currentSprite->setFrameRateChange(currentSprite->getFrameRateChange() - 1);
                  }
               }
               else
               {
                  if (currentSprite != NULL)
                  {
                     std::vector<ImageStruct> imageStructs = currentSprite->getImageStructs();

                     imageStructs[currentMenuItem - 1].duration--;

                     currentSprite->setImageStructs(imageStructs);
                  }
               }
            }
         }
      }
      else if (key[KEY_PGUP])
      {
         if (!keyDown)
         {
            keyDown = true;

            if (currentMode == Shadow)
            {
               shadowBackground = !shadowBackground;
            }
            else
            {
               if (currentMenuItem == 0)
               {
                  if (currentSprite != NULL)
                  {
                     currentSprite->setFrameRateChange(currentSprite->getFrameRateChange() + 1);
                  }
               }
               else
               {
                  if (currentSprite != NULL)
                  {
                     std::vector<ImageStruct> imageStructs = currentSprite->getImageStructs();

                     imageStructs[currentMenuItem - 1].duration++;

                     currentSprite->setImageStructs(imageStructs);
                  }
               }
            }
         }
      }
      else if (key[KEY_DEL])
      {
         if (!keyDown)
         {
            keyDown = true;

            if (currentMode == Shadow)
            {
               clear_bitmap(shadow);
               rectfill(shadow, 0, 0, shadow->w, shadow->h, makecol(255, 0, 255));
            }
            else
            {
               if (currentSprite != NULL)
               {
                  std::vector<ImageStruct> imageStructs = currentSprite->getImageStructs();

                  if (currentMenuItem - 1 < imageStructs.size())
                  {
                     imageStructs.erase(imageStructs.begin() + currentMenuItem - 1);

                     currentSprite->setImageStructs(imageStructs);
                  }
               }
            }
         }
      }
      else
      {
         keyDown = false;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> SpriteWindow::makeObjectPlacementItems()
{
   std::vector<std::string> menuItems;
   
   currentSpriteNames.clear();
   
   // (U) Get all sprites from common objects folder
   std::vector<std::string> commonSprites = getdir(sceneWindow.sceneManager.getSpriteManager().commonSpriteFolder + "/", "");

   for (int i = 0 ; i < commonSprites.size() ; i++)
   {
      menuItems.push_back(commonSprites[i]);
      currentSpriteNames.push_back(commonSprites[i]);
   }

   std::vector<std::string> sceneNames = getdir("Scenes/", "");
   
   // (U) Get all scene objects from specific scenes
   for (int i = 0 ; i < sceneNames.size() ; i++)
   {
      std::vector<std::string> sceneSpriteNames = getdir(sceneWindow.sceneManager.sceneFolder + "/" + sceneNames[i] + "/" + sceneWindow.sceneManager.getSpriteManager().spriteFolder + "/", "");
      
      for (int j = 0 ; j < sceneSpriteNames.size() ; j++)
      {
         std::cout << sceneSpriteNames[j] << std::endl;
         menuItems.push_back(sceneSpriteNames[j]);
         currentSpriteNames.push_back(sceneSpriteNames[j]);
      }
   }
   
   return menuItems;
}

////////////////////////////////////////////////////////////////////////////////

void SpriteWindow::drawGrid(BITMAP* backBuffer, double dt)
{
   int depth = 10;
   int multiple = 2;

   switch(direction)
   {
      case Down:
      {
         currentGroundPixel += dt * currentSprite->getFrameRateChange();
         
         for (int m = 0 ; m < 5 ; m++)
         {
            for (int i = 0, j = static_cast<int>(currentGroundPixel) ; i < 100 ; i++, j++)
            {
               for (int k = 1 ; k < depth ; k++)
               {
                  if (j % static_cast<int>((pow(multiple, k))) == 0)
                  {
                     putpixel(backBuffer, 370 - k - (m * 20), i + 190, 0);
                  }
               }
            }
         }
         
         break;
      }
      case Up:
      {
         currentGroundPixel -= dt * currentSprite->getFrameRateChange();
         
         for (int m = 0 ; m < 5 ; m++)
         {
            for (int i = 0, j = static_cast<int>(currentGroundPixel) ; i < 100 ; i++, j++)
            {
               for (int k = 1 ; k < depth ; k++)
               {
                  if (j % static_cast<int>((pow(multiple, k))) == 0)
                  {
                     putpixel(backBuffer, 370 - k - (m * 20), i + 190, 0);
                  }
               }
            }
         }
         
         break;
      }
      case Right:
      {
         currentGroundPixel += dt * currentSprite->getFrameRateChange();
         
         for (int m = 0 ; m < 5 ; m++)
         {
            for (int i = 270, j = static_cast<int>(currentGroundPixel) ; i < 370 ; i++, j++)
            {
               for (int k = 1 ; k < depth ; k++)
               {
                  if (j % static_cast<int>((pow(multiple, k))) == 0)
                  {
                     putpixel(backBuffer, i, k - 1 + (m * 20) + 190, 0);
                  }
               }
            }
         }
         
         break;
      }
      case Left:
      {
         currentGroundPixel -= dt * currentSprite->getFrameRateChange();
         
         for (int m = 0 ; m < 5 ; m++)
         {
            for (int i = 320, j = static_cast<int>(currentGroundPixel) ; i < 420 ; i++, j++)
            {
               for (int k = 1 ; k < depth ; k++)
               {
                  if (j % static_cast<int>((pow(multiple, k))) == 0)
                  {
                        putpixel(backBuffer, i, k - 1 + (m * 20) + 190, 0);
                  }
               }
            }
         }
         
         break;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SpriteWindow::xorShadow()
{
   int spritex = 320 - currentSprite->getWidth() / 2;
   int spritey = 240 - currentSprite->getHeight() / 2;

   for (int x = spritex ; x < currentSprite->getWidth() + spritex ; x++)
   {
      for (int y = spritey ; y < currentSprite->getHeight() + spritey ; y++)
      {
         // (U) Not transparent in current sprite
         if (makecol(255, 0, 255) != getpixel(currentSprite->getCurrentImage(), x - spritex, y - spritey))
         {
            // (U) Make transparent in shadow
            putpixel(shadow, x, y, makecol(255, 0, 255));
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

BITMAP* SpriteWindow::cropShadow(Point& upperLeft)
{
   int x1, x2, y1, y2;

   bool setX1 = false;
   bool setX2 = false;
   bool clearColumn = true;

   for (int x = 0 ; !setX1 && x < shadow->w ; x++)
   {
      clearColumn = true;

      for (int y = 0 ; clearColumn && y < shadow->h ; y++)
      {
         if (getpixel(shadow, x, y) != makecol(255, 0, 255))
         {
            clearColumn = false;
         }
      }

      if (!clearColumn)
      {
         setX1 = true;
         x1 = x;
      }
   }

   for (int x = shadow->w - 1 ; !setX2 && x >= 0 ; x--)
   {
      clearColumn = true;

      for (int y = 0 ; clearColumn && y < shadow->h ; y++)
      {
         if (getpixel(shadow, x, y) != makecol(255, 0, 255))
         {
            clearColumn = false;
         }
      }

      if (!clearColumn)
      {
         setX2 = true;
         x2 = x + 1;
      }
   }

   bool setY1 = false;
   bool setY2 = false;
   bool clearRow = true;

   for (int y = 0 ; !setY1 && y < shadow->h ; y++)
   {
      clearRow = true;

      for (int x = 0 ; clearRow && x < shadow->w ; x++)
      {
         if (getpixel(shadow, x, y) != makecol(255, 0, 255))
         {
            clearRow = false;
         }
      }

      if (!clearRow)
      {
         setY1 = true;
         y1 = y;
      }
   }

   for (int y = shadow->h - 1 ; !setY2 && y >= 0 ; y--)
   {
      clearRow = true;

      for (int x = 0 ; clearRow && x < shadow->w ; x++)
      {
         if (getpixel(shadow, x, y) != makecol(255, 0, 255))
         {
            clearRow = false;
         }
      }

      if (!clearRow)
      {
         setY2 = true;
         y2 = y + 1;
      }
   }

   if (x1 > x2 || y1 > y2)
   {
      x1 = 0;
      x2 = 0;
      y1 = 0;
      y2 = 0;
   }

   std::cout << x1 << ", " << y1 << std::endl;
   std::cout << x2 << ", " << y2 << std::endl;

   upperLeft.x = x1;
   upperLeft.y = y1;

   return create_sub_bitmap(shadow, x1, y1, x2 - x1, y2 - y1);
}
