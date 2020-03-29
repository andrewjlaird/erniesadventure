#include "BaseWindow.h"

BaseWindow::BaseWindow()
 : active(false)
{
}

////////////////////////////////////////////////////////////////////////////////

BaseWindow::~BaseWindow()
{
}

////////////////////////////////////////////////////////////////////////////////

void BaseWindow::setActive(bool inActive)
{
   active = inActive;
}

////////////////////////////////////////////////////////////////////////////////
      
bool BaseWindow::isActive()
{
   return active;
}
