#include "tablescontroller.h"

TablesController *TablesController::singleton=nullptr;

// Membro statico del singleton
TablesController *TablesController::getInstance()
{
    if (singleton == nullptr)
          singleton= new TablesController;

        return singleton;
}
TablesController::TablesController()
{
}
// Releases the singleton instance of this controller.

void TablesController::destroy() {

           if (singleton != 0) {
               delete singleton;
               singleton= 0;
           }

}

leadata* TablesController::writeValues(classData myClassData)
{
    return myLeaTable->writeValues(myClassData);
}
