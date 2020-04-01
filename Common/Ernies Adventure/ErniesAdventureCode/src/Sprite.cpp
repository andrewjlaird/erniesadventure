#include <iostream>
#include <fstream>
#include <sstream>

#include "Sprite.h"
#include "SpriteManager.h"

Sprite::Sprite(SpriteManager& inSpriteManager,
               std::string inSpriteFolder,
               std::string inSpriteName,
               unsigned short inX,
               unsigned short inY,
               double inFrameRateChange)
 : LayeredObject(inSpriteFolder + "/" + inSpriteName + ".bdr", inSpriteFolder + "/" + inSpriteName + "Shadow.bmp", inX, inY),
   spriteManager(inSpriteManager),
   onScreen(false),
   isInGame(false),
   spriteIsRunning(false),
   spriteFolder(inSpriteFolder),
   spriteFileName(inSpriteName + ".spt"),
   spriteName(inSpriteName),
   onScreenName(""),
   timeOnFrame(0),
   currentFrame(0),
   frameRateChange(inFrameRateChange),
   id(0)
{
}

////////////////////////////////////////////////////////////////////////////////

Sprite::~Sprite()
{
}

////////////////////////////////////////////////////////////////////////////////

bool Sprite::loadSprite()
{
   bool status = true;
   
   // (U) Load up sprite path and read in images file paths and durations.
   
   std::vector<std::string> imageFilePaths;
   std::vector<std::string> imageNames;
   std::vector<long> imageDurations;

   std::string line;
   std::string spriteFilePath = spriteFolder + "/" + spriteFileName;
   std::ifstream myfile(spriteFilePath.c_str());
   
   if (myfile.is_open())
   {
      // sprite name and pps
      std::getline (myfile, line);
      
      if (!myfile.fail())
      {
         // Try double first and if that fails, assume it's got a name
         std::stringstream ss;
         ss << line;
         double pixelsPerSecond;
         ss >> pixelsPerSecond;
         
         if (!ss.fail())
         {
            frameRateChange = pixelsPerSecond;
         }
         else
         {
            // If the double parse fails, then assume it's the name
            ss.str("");
            ss.clear();
            ss << line;
            std::string name;
            ss >> name;
         
            if (!ss.fail())
            {
               onScreenName = name;
            }
            
            // Now parse the pps
            std::getline (myfile, line);
            
            if (!myfile.fail())
            {
               ss.str("");
               ss.clear();
               ss << line;
               double pixelsPerSecond;
               ss >> pixelsPerSecond;

               if (!ss.fail())
               {
                  frameRateChange = pixelsPerSecond;
               }
            }
         }
      }
      
      // sprite images
      while (!myfile.fail())
      { 
         std::getline (myfile, line);
   
         std::stringstream ss;
         ss << line;
         
         long duration;
         std::string fileName;
   
         ss >> duration;
         ss >> fileName;
         
         std::string filePath = spriteFolder + "/" + fileName;
         
         if (!ss.fail() && !myfile.fail())
         {
            imageFilePaths.push_back(filePath);
            imageNames.push_back(fileName);
            imageDurations.push_back(duration);
         }
      }
      
      myfile.close();
   }
   else
   {
      std::cerr << "ERROR: Attempt to open sprite file failed: " << spriteFilePath << std::endl;
      status = false;
   }

   status = (imageFilePaths.size() > 0 && imageDurations.size() > 0);
   
   if (!status)
   {
      std::cerr << "ERROR: Image files or image durations didn't load" << std::endl;
   }
   else
   {
      imageStructs.clear();
   }

   for (int i = 0 ; status && i < imageFilePaths.size() && i < imageDurations.size() && i < imageNames.size() ; i++)
   {
      BITMAP* newBitmap = spriteManager.loadBitmap(imageFilePaths[i]);
      
      if (newBitmap == NULL)
      {
         std::cerr << "ERROR: Could not load image for sprite - aborting: " << imageFilePaths[i] << std::endl;
         status = false;
      }
      else
      {
         ImageStruct imageStruct;
         imageStruct.image = newBitmap;
         imageStruct.imageName = imageNames[i];
         imageStruct.duration = imageDurations[i];
         
         imageStructs.push_back(imageStruct);
      }
   }

   status = status && loadBoundaryFile();

   if (getShadowLayer() != NULL)
   {
      for (int i = 0 ; i < imageStructs.size() ; i++)
      {
         BITMAP* whiteShadow = create_bitmap(getShadowLayer()->w, getShadowLayer()->h);
         rectfill(whiteShadow, 0, 0, whiteShadow->w, whiteShadow->h, makecol(255, 0, 255));
         draw_sprite(whiteShadow, getShadowLayer(), 0, 0);

         whiteShadow = xorBitmaps(whiteShadow, imageStructs[i].image, getShadowOffset());

         /*PALETTE pal;
         get_palette(pal);
         save_bitmap("whiteShadow.bmp", whiteShadow, pal);

         COLOR_MAP trans_table;

         create_trans_table(&trans_table, default_palette, 0, 0, 0, NULL);

         set_trans_blender(0, 0, 0, 0);

         color_map = &trans_table;*/

         //draw_trans_sprite(imageStructs[i].image, whiteShadow, getShadowOffset().x, getShadowOffset().y);

         //save_bitmap("image.bmp", imageStructs[i].image, pal);

         destroy_bitmap(whiteShadow);
      }

      /*COLOR_MAP trans_table;

      create_trans_table(&trans_table, default_palette, 0, 0, 0, NULL);

      set_trans_blender(0, 0, 0, 64);

      color_map = &trans_table;*/
   }

   return status;
}

////////////////////////////////////////////////////////////////////////////////

void Sprite::setOnScreen(bool inOnScreen)
{
   onScreen = inOnScreen;
}

////////////////////////////////////////////////////////////////////////////////

bool Sprite::isOnScreen()
{
   return onScreen && isInGame;
}

////////////////////////////////////////////////////////////////////////////////

void Sprite::setInGame(bool inInGame)
{
   isInGame = inInGame;
}

////////////////////////////////////////////////////////////////////////////////

bool Sprite::inGame()
{
   return isInGame;
}

////////////////////////////////////////////////////////////////////////////////

void Sprite::setSpriteRunning(bool spriteRunning)
{
   spriteIsRunning = spriteRunning;
}

////////////////////////////////////////////////////////////////////////////////      
      
bool Sprite::isSpriteRunning()
{
   return spriteIsRunning;
}

////////////////////////////////////////////////////////////////////////////////

bool Sprite::requiresService()
{
   return frameRateChange > 0 && imageStructs.size() > 1;
}

////////////////////////////////////////////////////////////////////////////////

void Sprite::timeChange(double dt)
{
   if (spriteIsRunning)
   {
      timeOnFrame += dt;
   
      if (frameRateChange > 0)
      {
         if (imageStructs[currentFrame].duration != -1)
         {
            if (timeOnFrame > imageStructs[currentFrame].duration / frameRateChange)
            {
               timeOnFrame = timeOnFrame - imageStructs[currentFrame].duration / frameRateChange;

               if (currentFrame + 1 >= imageStructs.size())
               {
                  currentFrame = 0;
               }
               else
               {
                  currentFrame++;
               }
            }
         }
         else
         {
            timeOnFrame = 0;
         }
      }
      else
      {
         currentFrame = 0;
         timeOnFrame = 0;
      }
   }
   else
   {
      currentFrame = 0;
      timeOnFrame = 0;
   }
   
   doTimeChange(dt);
}

////////////////////////////////////////////////////////////////////////////////

void Sprite::doTimeChange(double dt)
{
   // (U) Do nothing
}

////////////////////////////////////////////////////////////////////////////////

BITMAP* Sprite::getCurrentImage()
{
   BITMAP* toReturn = NULL;

   if (imageStructs.size() > currentFrame)
   {
      toReturn = imageStructs[currentFrame].image;
   }

   return toReturn;
}

std::string Sprite::getOnScreenName()
{
   return onScreenName;
}

////////////////////////////////////////////////////////////////////////////////

void Sprite::setId(SpriteIdType newId)
{
   id = newId;
}

////////////////////////////////////////////////////////////////////////////////      

SpriteIdType Sprite::getId()
{
   return id;
}

////////////////////////////////////////////////////////////////////////////////

std::string Sprite::getName()
{
   return spriteName;
}

////////////////////////////////////////////////////////////////////////////////

int Sprite::getWidth()
{
   int width = 0;
   
   if (imageStructs.size() > 0 && imageStructs[0].image != NULL)
   {
      width = imageStructs[0].image->w;
   }
   
   return width;
}

////////////////////////////////////////////////////////////////////////////////      

int Sprite::getHeight()
{
   int height = 0;
   
   if (imageStructs.size() > 0 && imageStructs[0].image != NULL)
   {
      height = imageStructs[0].image->h;
   }
   
   return height;
}

////////////////////////////////////////////////////////////////////////////////

double Sprite::getFrameRateChange()
{
   return frameRateChange;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<ImageStruct> Sprite::getImageStructs()
{
   return imageStructs;
}

////////////////////////////////////////////////////////////////////////////////

std::string Sprite::getSpriteFolder()
{
   return spriteFolder;
}

////////////////////////////////////////////////////////////////////////////////

unsigned short Sprite::getCurrentFrame()
{
   return currentFrame;
}

////////////////////////////////////////////////////////////////////////////////

double Sprite::getTimeOnFrame()
{
   return timeOnFrame;
}







// (U) DEVELOPMENT
////////////////////////////////////////////////////////////////////////////////

bool Sprite::saveSprite(int xOffset, int yOffset)
{
   bool status = true;

   std::string spriteFilePath = spriteFolder + "/" + spriteFileName;
   std::ofstream myfile(spriteFilePath.c_str());
   if (myfile.is_open())
   {
      myfile << frameRateChange << std::endl;

      for (int i = 0 ; i < imageStructs.size() ; i++)
      {
         myfile << imageStructs[i].duration << " " << imageStructs[i].imageName << std::endl;
      }

      myfile.close();
   }
   else
   {
      std::cerr << "ERROR: Attempt to save sprite file failed: " << spriteFilePath << std::endl;
      status = false;
   }

   return status && saveSceneBoundaryFile(xOffset, yOffset);
}


////////////////////////////////////////////////////////////////////////////////

void Sprite::setCurrentFrame(unsigned short inCurrentFrame)
{
   currentFrame = inCurrentFrame;
}

////////////////////////////////////////////////////////////////////////////////

void Sprite::setTimeOnFrame(double inTimeOnFrame)
{
   timeOnFrame = inTimeOnFrame;
}

////////////////////////////////////////////////////////////////////////////////

BITMAP* Sprite::xorBitmaps(BITMAP* shadow, BITMAP* image, Point offset)
{
   for (int x = 0 ; x < shadow->w ; x++)
   {
      for (int y = 0 ; y < shadow->h ; y++)
      {
         // (U) Point in image is transparent
         if (x + offset.x < image->w && y + offset.y < image->h && x + offset.x >= 0 && y + offset.y >= 0 &&
             makecol(255, 0, 255) != getpixel(image, x + offset.x, y + offset.y) &&
             makecol(255, 0, 255) != getpixel(shadow, x, y))
         {
            putpixel(shadow, x, y, makecol(128, 128, 128));
         }
         else
         {
            // (U) Make transparent in shadow
            putpixel(shadow, x, y, makecol(255, 0, 255));
         }
      }
   }

   return shadow;
}

////////////////////////////////////////////////////////////////////////////////

void Sprite::setImageStructs(std::vector<ImageStruct> inImageStruct)
{
   imageStructs = inImageStruct;
}

////////////////////////////////////////////////////////////////////////////////

void Sprite::setFrameRateChange(double inFrameRateChange)
{
   frameRateChange = inFrameRateChange;
}
