#ifndef LAYERED_OBJECT_H
#define LAYERED_OBJECT_H

#include <allegro.h>
#ifdef _WIN32
#include <winalleg.h>
#endif
#include <string>
#include <vector>

#include "CommonSettings.h"

class LayeredObject
{
   public:
      
      LayeredObject(std::string boundaryFilePath,
                    std::string shadowPath,
                    unsigned short x,
                    unsigned short y);
      
      ~LayeredObject();
      
      bool loadBoundaryFile();
      bool saveSceneBoundaryFile(int xOffset, int yOffset);
      
      // (U) Returns true, if any part of object2 is above this object
      bool isBehind(LayeredObject& object2);

      std::vector<short> getHeightVector();
      
      std::vector<Point>& getBoundaryVector();

      Point getShadowOffset();
      BITMAP* getShadowLayer();

      bool overlap(Square otherSquare);
      Square getBoundarySquare();
      Square getOffsetBoundarySquare();

      void addBoundaryPoint(Point newPoint);
      void clearBoundaryPoint(int x, int y);
      void clearBoundaryPoints();
      
      void addHeightPoint(Point newPoint);
      void clearHeightPoint(int x);
      void clearHeightPoints();

      void setBoundarySquare(Square boundarySquare);

      // (U) Checks against the current scene object's boundary array to see if this pixel
      // is a valid place to move to.
      bool canMoveHere(int x, int y);
      
      short getX();
      
      short getY();
      
      void setX(short newX);
      
      void setY(short newY);

      void setReferencePoint(Point refPoint);
      Point getReferencePoint();

      virtual BITMAP* getCurrentImage() = 0;
      
   protected:

      std::string boundaryFilePath;
      std::string shadowPath;

      // (U) Vector of heights, one for each pixel left to right
      std::vector<short> heights;
      
      // (U) An array of points that cannot be moved to.
      std::vector<Point> cannotMoveHere;

      short x;
      short y;

      Point shadowOffset;
      BITMAP* shadow;

      // (U) The coordinates that should be used to do "near" comparisons between
      // objects.
      Point referencePoint;

      // (U) Used to do character comparisons
      Square boundary;
};

#endif
