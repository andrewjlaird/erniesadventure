#include "SceneObjectWindow.h"
#include "SceneObject.h"
#include "SceneWindow.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

SceneObjectWindow::SceneObjectWindow(SceneWindow& inSceneWindow)
 : BaseWindow(),
   currentMode(Border),
   blueMouse(NULL),
   redMouse(NULL),
   currentObject(NULL),
   lastMouseZ(mouse_z),
   leftMouseDown(false),
   rightMouseDown(false),
   writeOn(false),
   lastClickedBorderPoint(-1, -1),
   lastClickedLayerPoint(-1, -1),
   rightMouseClickDownPoint(-1, -1),
   keyDown(false),
   sceneWindow(inSceneWindow)
{
   blueMouse = load_bitmap("SceneWriter2Code/Images/blueMouse.bmp", 0);
   redMouse = load_bitmap("SceneWriter2Code/Images/redMouse.bmp", 0);
   
   if (blueMouse == 0 ||
       redMouse == 0)
   {
      // Shut down Allegro
      allegro_exit();    
       
      std::cout << "Error creating mouse images\n";
       
      system("PAUSE");
      exit(1);
   }
}

////////////////////////////////////////////////////////////////////////////////

SceneObjectWindow::~SceneObjectWindow()
{
}

////////////////////////////////////////////////////////////////////////////////

void SceneObjectWindow::getNextFrame(BITMAP* frame)
{
   rectfill(frame, 0, 0, 640, 480, makecol(255,255,200));
   
   for (int i = 0 ; i < 3 ; i++)
   {
      for (int j = 0 ; j < 10 ; j++)
      {
         textprintf_ex(frame, font, j * 10 + i * 220, j * 50, makecol(200, 200, 200), -1, "Scene Object Window");         
      }
   }
   
   if (currentObject != NULL)
   {
      draw_sprite(frame, currentObject->getCurrentImage(), 0, 0);      

      std::vector<Point> boundaries = currentObject->getBoundaryVector();
   
      for (int i = 0 ; i < boundaries.size() ; i++)
      {
         putpixel( frame, boundaries[i].x, boundaries[i].y, makecol(0,0,255));
      }
      
      std::vector<short> heights = currentObject->getHeightVector();
      
      for (int i = 0 ; i < heights.size() ; i++)
      {
         if (heights[i] != -1)
         {
            putpixel( frame, i, heights[i], makecol(255,0,0));
         }
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
   }
}

////////////////////////////////////////////////////////////////////////////////
      
void SceneObjectWindow::process()
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
            currentMode = Layer;
            break;
         }
         case Layer:
         {
            currentMode = Border;
            break;
         }
      }
      
      writeOn = false;
      lastMouseZ = mouse_z;
   }
   
   if (!leftMouseDown && (mouse_b & 1))
   {
      leftMouseDown = true;
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
         }
      }
   }
   
   checkKeyboard();
}

////////////////////////////////////////////////////////////////////////////////

void SceneObjectWindow::mouseClickBorder()
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
      
      for(int i = 0 ; i < points.size() ; i++)
      {
         if (currentObject != NULL && points[i].x < currentObject->getWidth() && points[i].y < currentObject->getHeight())
         {
            currentObject->addBoundaryPoint(points[i]);
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

void SceneObjectWindow::mouseClickLayer()
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
      
      for(int i = 0 ; i < points.size() ; i++)
      {
         if (currentObject != NULL && points[i].x < currentObject->getWidth())
         {
            currentObject->addHeightPoint(points[i]);
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
   }
}

////////////////////////////////////////////////////////////////////////////////

void SceneObjectWindow::rightMouseClickBorder()
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
   
   for (int i = xMin ; i < xMax ; i++)
   {
      for (int j = yMin ; j < yMax ; j++)
      {
         if (currentObject != NULL && i < currentObject->getWidth() && j < currentObject->getHeight())
         {
            currentObject->clearBoundaryPoint(i, j);
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SceneObjectWindow::rightMouseClickLayer()
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
      
   for (int i = xMin ; i < xMax ; i++)
   {
      if (currentObject != NULL && i < currentObject->getWidth())
      {
         currentObject->clearHeightPoint(i);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SceneObjectWindow::checkKeyboard()
{
   if (key_shifts & KB_CTRL_FLAG)
   {
      // (U) Load scene object
      if (key[KEY_L])
      {
         if (!keyDown)
         {
            keyDown = true;
            
            int choice = userSelectFromMenu(makeObjectPlacementItems());
         
            if (choice != -1)
            {
               std::string objectName = currentObjectNames[choice];
               
               std::string imagePath = sceneWindow.sceneManager.commonSceneObjectFolder + "/" + objectName + "/" + objectName + ".bmp";   
               std::string boundariesPath = sceneWindow.sceneManager.commonSceneObjectFolder + "/" + objectName + "/" + objectName + ".bdr";
               
               struct stat stFileInfo;
               
               // (U) If it isn't in the common folder either, then it doesn't exist
               if (stat(imagePath.c_str(),&stFileInfo) != 0 || stat(boundariesPath.c_str(),&stFileInfo) != 0)
               {
                  bool match = false;

                  std::vector<std::string> sceneNames = sceneWindow.sceneManager.getSceneNames();
                  
                  for (int i = 0 ; !match && i < sceneNames.size() ; i++)
                  {
                     imagePath = sceneWindow.sceneManager.sceneFolder + "/" + sceneNames[i] + "/" + sceneWindow.sceneManager.sceneObjectFolder + "/" + objectName + "/" + objectName + ".bmp";   
                     boundariesPath = sceneWindow.sceneManager.sceneFolder + "/" + sceneNames[i] + "/" + sceneWindow.sceneManager.sceneObjectFolder + "/" + objectName + "/" + objectName + ".bdr";
            
                     // (U) Attempt to get the file attributes
                     // If the attributes can't be retrieved, then try the common folder
                     if (stat(imagePath.c_str(),&stFileInfo) == 0 && stat(boundariesPath.c_str(),&stFileInfo) == 0)
                     {
                        match = true;
                     }
                  }
               }
               
               std::cerr << "imagePath: " << imagePath << std::endl;
               std::cerr << "boundariesPath: " << boundariesPath << std::endl;
               
               currentObject = new SceneObject(imagePath,
                                               boundariesPath,
                                               objectName,
                                               false,
                                               0, 
                                               0);
                                               
               if (!currentObject->loadSceneObject())
               {
                  std::cerr << "ERROR: loading scene object: " << objectName << std::endl;

                  waitForAnyKey("ERROR: loading scene object: " + objectName);
               }
            }
            else
            {
               std::cout << "Could not find scene object" << std::endl;
               waitForAnyKey("Could not find scene object");
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
            
            currentObject->saveSceneBoundaryFile();
            
            sceneWindow.reloadSceneObject(currentObject->getId());
         }
      }
      else if(key[KEY_N])
      {
         if (!keyDown)
         {
            keyDown = true;
            
            // (U) New scene object
            std::string newSceneObject = readString("Enter name of new scene object");

            std::string commonObject = readString("C for common folder - S for specific scene folder");

            std::string baseFolder = sceneWindow.sceneManager.commonSceneObjectFolder + "/";

            // (U) Check that scene exists
            bool validBaseFolder = false;

            if (commonObject.compare("S") == 0)
            {
               std::string sceneName = readString("Enter scene name");
               
               std::vector<std::string> sceneNames = getdir(sceneWindow.sceneManager.sceneFolder + "/", "");
               
               for (int i = 0 ; !validBaseFolder && i < sceneNames.size() ; i++)
               {
                  if (sceneNames[i].compare(sceneName) == 0)
                  {
                     validBaseFolder = true;
                  }
               }
               
               if (!validBaseFolder)
               {
                  waitForAnyKey("That scene doesn't exist");
               }
               else
               {
                  baseFolder = std::string(sceneWindow.sceneManager.sceneFolder + "/" + sceneName + "/" + sceneWindow.sceneManager.sceneObjectFolder + "/");
               }
            }
            else
            {
               baseFolder = sceneWindow.sceneManager.commonSceneObjectFolder + "/";
               validBaseFolder = true;
            }

            if (validBaseFolder)
            {
               // (U) Check that scene object doesn't already exists
               bool exists = false;
               
               std::vector<std::string> sceneObjects = getdir(baseFolder, "");
             
               for (int i = 0 ; !exists && i < sceneObjects.size() ; i++)
               {
                  if (sceneObjects[i].compare(newSceneObject) == 0)
                  {
                     exists = true;
                  }
               }
               
               if (exists)
               {
                  waitForAnyKey("That scene object already exists");
               }
               else
               {
                  // (U) Create folder
                  if (mkdir(std::string(baseFolder + "/" + newSceneObject).c_str()) == -1)
                  {
                     waitForAnyKey("Error creating scene object folder");
                  }
                  else
                  {
                     // (U) Create bdr
                     // (U) Create bmp
                     std::ofstream bdrFile(std::string(baseFolder + "/" + newSceneObject + "/" + newSceneObject + ".bdr").c_str());
                     
                     BITMAP *bmp;
                     PALETTE pal;
                     get_palette(pal);
                     bmp = create_bitmap(200, 50);
                     rect(bmp, 0, 0, 200, 50, makecol(0, 0, 0));
                     textprintf_centre_ex(bmp, font, 100, 25, makecol(255, 255, 255), -1, newSceneObject.c_str());         
                     
                     save_bmp(std::string(baseFolder + "/" + newSceneObject + "/" + newSceneObject + ".bmp").c_str(), bmp, pal);
                     destroy_bitmap(bmp);
                     
                     waitForAnyKey("Successfully created new scene object: " + newSceneObject);
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

std::vector<std::string> SceneObjectWindow::makeObjectPlacementItems()
{
   std::vector<std::string> menuItems;
   
   currentObjectNames.clear();
   
   std::vector<std::string> sceneNames = getdir("Scenes/", "");
   
   // (U) Get all scene objects from common objects folder
   std::vector<std::string> commonSceneObjects = getdir(sceneWindow.sceneManager.commonSceneObjectFolder + "/", "");

   for (int i = 0 ; i < commonSceneObjects.size() ; i++)
   {
      menuItems.push_back(commonSceneObjects[i]);
      currentObjectNames.push_back(commonSceneObjects[i]);
   }
   
   // (U) Get all scene objects from specific scenes
   for (int i = 0 ; i < sceneNames.size() ; i++)
   {
      std::vector<std::string> currentSceneNames = getdir(sceneWindow.sceneManager.sceneFolder + "/" + sceneNames[i] + "/" + sceneWindow.sceneManager.sceneObjectFolder + "/", "");
      
      for (int j = 0 ; j < currentSceneNames.size() ; j++)
      {
         menuItems.push_back(currentSceneNames[j]);
         currentObjectNames.push_back(currentSceneNames[j]);
      }
   }
   
   return menuItems;
}
