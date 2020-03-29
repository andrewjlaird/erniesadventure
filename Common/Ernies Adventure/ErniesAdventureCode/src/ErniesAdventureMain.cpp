#include <cstdlib>
#include <iostream>
#include <allegro.h>

#include "DisplayDriver.h"
#include "SceneManager.h"

#include "DisplayTextAction.h"

using namespace std;

int main(int argc, char *argv[])
{
   allegro_init(); 
   install_keyboard(); 
   install_timer();

   srand(time(NULL));

   // Redirect stderr to output.log   
   freopen ("output.log","w",stderr);

   std::cerr << "Ernie's Adventure Output" << std::endl;
   std::cerr << std::endl << "STARTING LOAD SEQUENCE" << std::endl << std::endl;

   bool fullScreen = true;

   for (int i = 0 ; i < argc ; i++)
   {
      if ((std::string(argv[i])).compare("-windowed") == 0)
      {
         fullScreen = false;
      }
   }
   
   fullScreen = false;

   // (U) These brackets ensure that the destructors are called on all these
   // objects before we call allegro_exit().
   {
      DisplayDriver displayDriver(fullScreen);
      
      displayDriver.initializeDisplay();
      
      SceneManager sceneManager(displayDriver);

      std::cerr << std::endl << "FINISHED LOAD SEQUENCE" << std::endl << std::endl;

      sceneManager.run();
   }

   allegro_exit();

   system("PAUSE");

   return 0;
}

END_OF_MAIN();
