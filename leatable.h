#ifndef LEATABLE_H
#define LEATABLE_H

#include "leadata.h"
#include <QtGui>

class LeaTable
{
public:
    LeaTable();
    leadata* writeValues(classData);
    leadata* getLeaData_1_1M();
    leadata* getLeaData_3R();
    leadata* getLeaData_3B();

private:
    leadata *LeaValues_1_1M;
    leadata *LeaValues_3R;
    leadata *LeaValues_3B;
};

#endif // LEATABLE_H
