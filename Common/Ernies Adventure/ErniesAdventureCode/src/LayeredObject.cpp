#include <iostream>
#include <fstream>
#include <sstream>

#include "LayeredObject.h"

LayeredObject::LayeredObject(std::string inBoundaryFilePath,
                             std::string inShadowPath,
                             unsigned short inX,
                             unsigned short inY)
 : boundaryFilePath(inBoundaryFilePath),
   shadowPath(inShadowPath),
   x(inX),
   y(inY),
   referencePoint(inX, inY),
   boundary(Point(-1, -1), Point(-1, -1)),
   shadowOffset(-1, -1),
   shadow(NULL)
{
}

////////////////////////////////////////////////////////////////////////////////

LayeredObject::~LayeredObject()
{
}

////////////////////////////////////////////////////////////////////////////////

bool LayeredObject::loadBoundaryFile()
{
   bool status = true;
   
   cannotMoveHere.clear();
   heights.clear();
   
   std::string line;
   std::ifstream myfile(boundaryFilePath.c_str());
   if (myfile.is_open())
   {
      std::getline (myfile, line);

      std::stringstream ss;
      ss << line;
      
      short nextX;
      char commaChar;
      short nextY;

      while (!ss.fail())
      {      
         ss >> nextX;
         ss >> commaChar;
         ss >> nextY;
         
         Point nextPoint(nextX, nextY);
         
         if (!ss.fail())
         {
            cannotMoveHere.push_back(nextPoint);
         }
      }
      
      std::getline (myfile, line);

      ss.str("");
      ss.clear();
      ss << line;
      
      short nextHeight;

      for (int i = 0 ; !ss.fail() ; i++)
      {      
         ss >> nextHeight;
         
         if (!ss.fail())
         {
            heights.push_back(nextHeight);
         }
      }


      std::getline (myfile, line);

      ss.str("");
      ss.clear();
      ss << line;

      int offsetX;
      int offsetY;

      ss >> offsetX;
      ss >> commaChar;
      ss >> offsetY;

      if (!myfile.fail() && !ss.fail())
      {
         referencePoint = Point(offsetX, offsetY);
      }
      else
      {
         referencePoint = Point(0, 0);
      }


      // Boundary square if this is used as a moving object
      std::getline (myfile, line);

      ss.str("");
      ss.clear();
      ss << line;

      int upperLeftX;
      int upperLeftY;
      int lowerRightX;
      int lowerRightY;

      ss >> upperLeftX;
      ss >> commaChar;
      ss >> upperLeftY;
      ss >> commaChar;
      ss >> lowerRightX;
      ss >> commaChar;
      ss >> lowerRightY;

      if (!myfile.fail() && !ss.fail())
      {
         boundary = Square(Point(upperLeftX, upperLeftY), Point(lowerRightX, lowerRightY));
      }


      // Shadow if available
      ss.str("");
      ss.clear();
      ss << line;

      int shadowX;
      int shadowY;

      ss >> shadowX;
      ss >> commaChar;
      ss >> shadowY;

      if (!ss.fail())
      {
         shadowOffset = Point(shadowX, shadowY);
      }

      myfile.close();
   }
   else
   {
      status = false;
   }

   shadow = load_bitmap(shadowPath.c_str(), NULL);

   // It's not an error if shadow doesn't load - there might not be one

   return status;
}

////////////////////////////////////////////////////////////////////////////////

bool LayeredObject::saveSceneBoundaryFile(int xOffset, int yOffset)
{
   bool status = true;
   
   std::ofstream myfile(boundaryFilePath.c_str());
   if (myfile.is_open())
   {
      for (int i = 0 ; i < cannotMoveHere.size() ; i++)
      {
         myfile << cannotMoveHere[i].x << "," << cannotMoveHere[i].y << " ";
      }
      
      myfile << std::endl;

      for (int i = 0 ; i < heights.size() ; i++)
      {
         myfile << heights[i] << " ";
      }

      myfile << std::endl;

      if (referencePoint.x != -1 && referencePoint.y != -1)
      {
         myfile << referencePoint.x << "," << referencePoint.y;
      }

      myfile << std::endl;

      if (boundary.upperLeft.x != -1 && boundary.upperLeft.y != -1 &&
          boundary.lowerRight.x != -1 && boundary.lowerRight.y != -1)
      {
         myfile << boundary.upperLeft.x << "," << boundary.upperLeft.y << "," << boundary.lowerRight.x << "," << boundary.lowerRight.y;
      }
      
      if (xOffset != -1 && yOffset != -1)
      {
         myfile << xOffset << "," << yOffset << std::endl;
      }

      myfile.close();
   }
   else
   {
      std::cerr << "ERROR: Attempt to open boundary file failed: " << boundaryFilePath << std::endl;
      status = false;
   }
   
   return status;
}

////////////////////////////////////////////////////////////////////////////////

short LayeredObject::getX()
{
   return x;
}

////////////////////////////////////////////////////////////////////////////////

short LayeredObject::getY()
{
   return y;
}

////////////////////////////////////////////////////////////////////////////////

void LayeredObject::setX(short newX)
{
   x = newX;
}

////////////////////////////////////////////////////////////////////////////////
      
void LayeredObject::setY(short newY)
{
   y = newY;
}

////////////////////////////////////////////////////////////////////////////////

Point LayeredObject::getReferencePoint()
{
   return referencePoint;
}

////////////////////////////////////////////////////////////////////////////////

void LayeredObject::setReferencePoint(Point refPoint)
{
   referencePoint = refPoint;
}

////////////////////////////////////////////////////////////////////////////////

bool LayeredObject::isBehind(LayeredObject& object2)
{
   bool isBehind = false;
   bool overlaps = false;
   
   std::vector<short> object2Heights = object2.getHeightVector();
   
   for (int i = 0 ; !isBehind && i < object2Heights.size() ; i++)
   {
      // (U) Calculate which index in our height vector this pixel should
      // test against
      int ourHeightIndex = object2.getX() + i - x;
      
      // (U) Check if this pixel is somewhere in our object
      if (ourHeightIndex >= 0 && ourHeightIndex < heights.size())
      {
         // (U) Check if this pixel is behind (above) our pixel
         if (object2Heights[i] != -1 &&
             heights[ourHeightIndex] != -1)
         {
            overlaps = true;
            
            if (object2.getY() + object2Heights[i] < y + heights[ourHeightIndex])
            {
               isBehind = true;
            }
         }
      }
   }

   // (U) In this case, just flag it as behind if object 2's reference y is above
   // this reference y.
   if (!overlaps)
   {
      if (object2.getY() + object2.getReferencePoint().y < y + referencePoint.y)
      {
         isBehind = true;
      }
   }
   
   return isBehind;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<short> LayeredObject::getHeightVector()
{
   return heights;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<Point>& LayeredObject::getBoundaryVector()
{
   return cannotMoveHere;
}

////////////////////////////////////////////////////////////////////////////////

Point LayeredObject::getShadowOffset()
{
   return shadowOffset;
}

////////////////////////////////////////////////////////////////////////////////

BITMAP* LayeredObject::getShadowLayer()
{
   return shadow;
}

////////////////////////////////////////////////////////////////////////////////

Square LayeredObject::getOffsetBoundarySquare()
{
   return boundary + Point(x, y);
}

////////////////////////////////////////////////////////////////////////////////

bool LayeredObject::overlap(Square otherBoundary)
{
   return boundary.overlap(otherBoundary);
}

////////////////////////////////////////////////////////////////////////////////

Square LayeredObject::getBoundarySquare()
{
   return boundary;
}

////////////////////////////////////////////////////////////////////////////////

void LayeredObject::addBoundaryPoint(Point newPoint)
{
   cannotMoveHere.push_back(newPoint);
}

////////////////////////////////////////////////////////////////////////////////

void LayeredObject::clearBoundaryPoint(int x, int y)
{
   bool found = false;
   
   do
   {   
      found = false;
      for (int i = 0 ; i < cannotMoveHere.size() ; i++)
      {
         if (cannotMoveHere[i].x == x && cannotMoveHere[i].y == y)
         {
            cannotMoveHere.erase(cannotMoveHere.begin() + i);
            found = true;
            break;
         }
      }
   }
   while (found);
}

////////////////////////////////////////////////////////////////////////////////

void LayeredObject::clearBoundaryPoints()
{
   cannotMoveHere.clear();
}

////////////////////////////////////////////////////////////////////////////////

void LayeredObject::addHeightPoint(Point newPoint)
{
   if(newPoint.x < heights.size())
   {
      heights[newPoint.x] = newPoint.y;
   }
   else
   {
      int difference = newPoint.x - heights.size();
      
      for (int i = 0 ; i <= difference ; i++)
      {
         heights.push_back(-1);
      }
      
      heights[newPoint.x] = newPoint.y;
   }
}

////////////////////////////////////////////////////////////////////////////////

void LayeredObject::clearHeightPoint(int x)
{
   if(x < heights.size())
   {
      heights[x] = -1;
   }
}

////////////////////////////////////////////////////////////////////////////////

void LayeredObject::clearHeightPoints()
{
   for (int i = 0 ; i < heights.size() ; i++)
   {
      heights[i] = -1;
   }
}

////////////////////////////////////////////////////////////////////////////////

void LayeredObject::setBoundarySquare(Square boundarySquare)
{
   boundary = boundarySquare;
}

////////////////////////////////////////////////////////////////////////////////

bool LayeredObject::canMoveHere(int inX, int inY)
{
   bool canMoveHere = true;
   
   for (int i = 0 ; canMoveHere && i < cannotMoveHere.size() ; i++)
   {
      if (cannotMoveHere[i].x + x == inX && cannotMoveHere[i].y + y == inY)
      {
         canMoveHere = false;
      }
   }
   
   return canMoveHere;
}
