#include "Inventory.h"
#include "CommonSettings.h"

#include <iostream>
#include <map>

Inventory::Inventory(std::string inInventoryPath)
 : inventoryPath(inInventoryPath)
{
   // (U) Make inventory list
   std::vector<std::string> itemNames = getdir(inventoryPath);

   // (U) Start with ID 1
   InventoryIdType nextId = 1;

   for (int i = 0 ; i < itemNames.size() ; i++)
   {
      InventoryObject nextObject;

      nextObject.image = load_bitmap(std::string(inventoryPath + "/" + itemNames[i] + "/" + itemNames[i] + ".bmp").c_str(), 0);

      if (nextObject.image != NULL)
      {
         std::cerr << "Registering inventory item: " << nextId << " - " << itemNames[i] << std::endl;

         nextObject.inInventory = false;
         nextObject.name = itemNames[i];

         inventoryItemToIdMap[nextId] = nextObject;

         nextId++;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

Inventory::~Inventory()
{
}

////////////////////////////////////////////////////////////////////////////////

bool Inventory::isInInventory(InventoryIdType inventoryId)
{
   bool toReturn = false;
   
   std::map<InventoryIdType, InventoryObject>::iterator iter = inventoryItemToIdMap.find(inventoryId);

   if (iter != inventoryItemToIdMap.end())
   {
      if (iter->second.inInventory)
      {
         toReturn = true;
      }
   }
   else
   {
      std::cerr << "ERROR: Inventory::isInInventory - Item not found in inventory: " << inventoryId << std::endl;
   }
   
   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

void Inventory::setInInventory(InventoryIdType inventoryId, bool isInInventory)
{
   std::map<InventoryIdType, InventoryObject>::iterator iter = inventoryItemToIdMap.find(inventoryId);

   if (iter != inventoryItemToIdMap.end())
   {
      iter->second.inInventory = isInInventory;
   }
   else
   {
      std::cerr << "ERROR: Inventory::setInInventory - Item not found in inventory: " << inventoryId << std::endl;
   }
}

////////////////////////////////////////////////////////////////////////////////

std::vector<InventoryObject> Inventory::getInventory()
{
   std::vector<InventoryObject> toReturn;
   
   for (std::map<InventoryIdType, InventoryObject>::iterator iter = inventoryItemToIdMap.begin() ;
        iter != inventoryItemToIdMap.end() ;
        iter++)
   {
      if (iter->second.inInventory)
      {
        toReturn.push_back(iter->second);
      }
   }
   
   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

InventoryIdType Inventory::inventoryItemNameToId(std::string name)
{
   InventoryIdType toReturn = 0;

   for (std::map<InventoryIdType, InventoryObject>::iterator iter = inventoryItemToIdMap.begin() ;
        iter != inventoryItemToIdMap.end() ;
        iter++)
   {
      if (iter->second.name.compare(name) == 0)
      {
        toReturn = iter->first;
      }
   }

   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

std::string Inventory::inventoryIdToName(InventoryIdType id)
{
   std::map<InventoryIdType, InventoryObject>::iterator iter = inventoryItemToIdMap.find(id);
   std::string toReturn = "InvalidInventoryId";

   if (iter != inventoryItemToIdMap.end())
   {
      toReturn = iter->second.name;
   }

   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

std::string Inventory::getSaveString()
{
   std::string toSave = "INVENTORY_OBJECTS\n";

   for (std::map<InventoryIdType, InventoryObject>::iterator iter = inventoryItemToIdMap.begin() ;
        iter != inventoryItemToIdMap.end() ;
        iter++)
   {
     toSave = toSave +
              inventoryIdToName(iter->first) + " " +
              BoolToString(isInInventory(iter->first)) + "\n";
   }
   
   return toSave;
}
