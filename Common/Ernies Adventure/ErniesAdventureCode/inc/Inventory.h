#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <vector>
#include <map>

#include <allegro.h>
#ifdef _WIN32
#include <winalleg.h>
#endif

typedef unsigned long InventoryIdType;

struct InventoryObject
{
   std::string name;
   bool inInventory;
   BITMAP* image;
};

class Inventory
{
   public:
      
      Inventory(std::string inventoryPath);
      
      ~Inventory();
      
      bool isInInventory(InventoryIdType inventoryId);

      void setInInventory(InventoryIdType inventoryId, bool isInInventory);

      InventoryIdType inventoryItemNameToId(std::string name);
      std::string inventoryIdToName(InventoryIdType id);
      
      std::vector<InventoryObject> getInventory();

      std::string getSaveString();

   private:
      
       std::map<InventoryIdType, InventoryObject> inventoryItemToIdMap;
       std::string inventoryPath;

};

#endif
