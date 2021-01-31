#ifndef TABLESCONTROLLER_H
#define TABLESCONTROLLER_H
#include "leatable.h"


class TablesController
{
public:
    TablesController();
    // Funzione membro del singleton
    static TablesController *getInstance();

    //Distrugge l'istanza del singleton
    static void destroy();
    leadata* writeValues(classData);

private:

  static TablesController *singleton; // pointer to the singleton

   LeaTable *myLeaTable;

};

#endif // TABLESCONTROLLER_H
