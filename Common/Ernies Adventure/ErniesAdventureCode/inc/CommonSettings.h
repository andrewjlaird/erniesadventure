#ifndef COMMON_SETTINGS_H
#define COMMON_SETTINGS_H

#include <string>
#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>

#include <allegro.h>
#ifdef _WIN32
#include <winalleg.h>
#endif
#include <vector>

static const double version = 1.0;

struct Point
{
   int x;
   int y;
   
   Point(int inX, int inY)
    : x(inX),
      y(inY)
   {
   }
   
   Point& operator=(const Point& rhs)
   {
      if (this != &rhs)
      {
         x = rhs.x;
         y = rhs.y;
      }
      
      return *this;
   }

   const Point operator+(const Point& point) const
   {
      Point result = *this;
      result.x += point.x;
      result.y += point.y;
      return result;
   }

   Point& operator+=(const Point& otherPoint)
   {
      x += otherPoint.x;
      y += otherPoint.y;
   }

   // (U) Left and above is less than
   bool operator<(const Point& p) const
   {
     return y < p.y && x < p.x;
   }
};

struct Square
{
   Point upperLeft;
   Point lowerRight;

   Square(Point inUpperLeft, Point inLowerRight)
   : upperLeft(inUpperLeft),
     lowerRight(inLowerRight)
   {

   }
    
   Square& operator=(const Square& rhs)
   {
      if (this != &rhs)
      {
         upperLeft = rhs.upperLeft;
         lowerRight = rhs.lowerRight;
      }
      
      return *this;
   }

   const Square operator+(const Point& point) const
   {
      Square result = *this;
      result.upperLeft += point;
      result.lowerRight += point;
      return result;
   }

   bool pointInSquare(int x, int y)
   {
      // (U) The point is in the square if it's x and y are within the bounds
      // of the square
      return x >= upperLeft.x &&
             y >= upperLeft.y &&
             x <= lowerRight.x &&
             y <= lowerRight.y;
   }

   bool pointInSquare(Point checkPoint)
   {
      return pointInSquare(checkPoint.x, checkPoint.y);
   }

   bool overlap(Square otherSquare)
   {      
      // (U) If any of these points are in the square, or the other squares points are in our square, then they overlap
      return pointInSquare(otherSquare.upperLeft) ||
             pointInSquare(otherSquare.lowerRight) ||
             pointInSquare(otherSquare.upperLeft.x, otherSquare.lowerRight.y) ||
             pointInSquare(otherSquare.lowerRight.x, otherSquare.upperLeft.y) ||
             otherSquare.pointInSquare(upperLeft) ||
             otherSquare.pointInSquare(lowerRight) ||
             otherSquare.pointInSquare(upperLeft.x, lowerRight.y) ||
             otherSquare.pointInSquare(lowerRight.x, upperLeft.y);
   }
};

// (U) Static function to convert a string to all upper case characters
static std::string StringToUpper(std::string myString)
{
  const int length = myString.length();
  
  for(int i=0; i!=length ; ++i)
  {
    myString[i] = std::toupper(myString[i]);
  }
  
  return myString;
}

// (U) Returns true if myString is any case of true
static bool StringToBool(std::string myString)
{
  return (StringToUpper(myString).compare("TRUE") == 0);
}

// (U) Returns "true" if myString is true, otherwise "false" is returned
static std::string BoolToString(bool toTest)
{
  return (toTest ? "true" : "false");
}

static bool debugFpsOn = false;
static bool debugBordersOn = false;
static bool debugRestOff = false;
static bool debugVsyncOff = false;
static bool debugXYOn = false;

static std::string readString(std::string prompt, std::string startingString = "")
{
   std::string line = startingString;
   int nextKey = 0;
   
   clear_keybuf();

   rectfill(screen, 0, 460, 640, 480, makecol(200,200,20));
   textprintf_ex(screen, font, 5, 465, makecol(0, 0, 0), -1, "%s > %s", prompt.c_str(), line.c_str());   
   
   do
   {
      nextKey = readkey();
      
      if (nextKey != 17165 && nextKey != 15131)
      {
         if (nextKey == 16136)
         {
            if (line.length() > 0)
            {
               line.resize(line.length() - 1);
            }
         }
         else
         {         
            line.push_back((char)(nextKey));
         }
         
         rectfill(screen, 0, 460, 640, 480, makecol(200,200,20));
         textprintf_ex(screen, font, 5, 465, makecol(0, 0, 0), -1, "%s > %s", prompt.c_str(), line.c_str());   
         textprintf_ex(screen, font, 600, 465, makecol(0, 0, 0), -1, "%d", nextKey);   
      }
   }
   while(nextKey != 17165 && nextKey != 15131);
   
   if (nextKey == 15131)
   {
      line = "";
   }
   
   return line;
}

static std::vector<std::string> getdir (std::string dir, std::string endingIn = "", bool includeHiddenFolders = false)
{
   DIR *dp;
   struct dirent *dirp;

   if( (dp = opendir(dir.c_str())) == NULL) 
   {
      std::cerr << "Error(" << errno << ") opening " << dir << std::endl;
   }
      
   std::vector<std::string> files;
      
   while ((dirp = readdir(dp)) != NULL) 
   {
      if (includeHiddenFolders || (std::string(dirp->d_name).length() > 0 && std::string(dirp->d_name)[0] != '.'))
      {      
         if (std::string(dirp->d_name) != "." && std::string(dirp->d_name) != "..")
         {
            if (std::string(dirp->d_name).substr(std::string(dirp->d_name).size() - endingIn.size()) == endingIn)
            {
               files.push_back(std::string(dirp->d_name));
            }
         }
      }
   }
   
   closedir(dp);
   
   return files;
}

static void recoloredArea(BITMAP* image, int x1, int y1, int x2, int y2)
{
   for (int i = y1 ; i <= y2 ; i++)
   {
      for (int j = x1 ; j <= x2 ; j++)
      {
         int color = getpixel(image, j, i);
         
         int red = getr32(color);
         int green = getg32(color);
         int blue = getb32(color);
         
         putpixel(image, j, i, makecol(red / 3, green / 3, blue / 3));
         //putpixel(image, j, i, makecol(255, 255, 255));
      }
   }
}

static std::vector<Point> lineToPoints(int x1, int y1, int x2, int y2)
{
   std::vector<Point> points;

   int minX;
   int maxX;
   int minY;
   int maxY;
   double currentY;
   
   if (x1 < x2)
   {
      minX = x1;
      maxX = x2;
      currentY = static_cast<double>(y1);
   }
   else
   {
      minX = x2;
      maxX = x1;
      currentY = static_cast<double>(y2);
   }
   
   if (y1 < y2)
   {
      minY = y1;
      maxY = y2;
   }
   else
   {
      minY = y2;
      maxY = y1;
   }
   
   int range = maxX - minX + 1;
   double deltaY = (maxY - minY + 1) / static_cast<double>(range);
   bool ascending = (static_cast<int>(currentY) == minY);
   
   if (deltaY <= 1)
   {
      for (int currentX = minX ; currentX <= maxX ; currentX++)
      {
         points.push_back(Point(currentX, static_cast<int>(currentY)));                  
         
         if (ascending)
         {
            currentY += deltaY;
         }
         else
         {
            currentY -= deltaY;
         }
      }
   }
   else
   {
      int range = maxY - minY + 1;
      double currentX;
      
      if (y1 < y2)
      {
         currentX = static_cast<double>(x1);
      }
      else
      {
         currentX = static_cast<double>(x2);
      }
      
      double deltaX = (maxX - minX + 1) / static_cast<double>(range);
      bool ascending = (static_cast<int>(currentX) == minX);
      
      for (int currentY = minY ; currentY <= maxY ; currentY++)
      {
         points.push_back(Point(static_cast<int>(currentX), currentY));                  
         
         if (ascending)
         {
            currentX += deltaX;
         }
         else
         {
            currentX -= deltaX;
         }
      }
   }
   
   return points;
}

#endif
