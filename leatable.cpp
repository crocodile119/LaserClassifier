#include "leatable.h"
#include "leadata.h"
#include <QTextStream>
#include <QDataStream>
#include <QFile>

LeaTable::LeaTable()
{
    LeaValues_1_1M=writeValues(CLASSE_1_1M);
    LeaValues_3R=writeValues(CLASSE_3R);
    LeaValues_3B=writeValues(CLASSE_3B);
}

leadata* LeaTable::writeValues(classData myClassData) {

    int _formulaNumber;
    double _wavelenght1;
    double _wavelenght2;
    double _time1;
    double _time2;
    int _effects;
    int _t;
    int _C1;
    int _C2;
    int _C3;
    int _C4;
    int _C5;
    int _C6;
    int _C7;
    double _formula;
    int _sort;
    int maxRow;
    QString filename;

    if(myClassData==CLASSE_1_1M)
    {
        maxRow=41;
        filename="CLASSE_1_1M_dataBinary.dat";
    }
    else if(myClassData==CLASSE_3R)
    {
        maxRow=38;
        filename="CLASSE_3R_dataBinary.dat";
    }
    else
    {
        maxRow=21;
        filename="CLASSE_3B_dataBinary.dat";
    }

    leadata* LeaValues= new leadata[maxRow];

    QFile fileBinary(filename);
    if (!fileBinary.open(QIODevice::ReadOnly))
    {
        qDebug("Can't open file");
        exit(1);
    }

    QDataStream in(&fileBinary);
    in.setVersion(QDataStream::Qt_4_3);

    for(int row=0; row<maxRow; row++)
    {

              in >> _formulaNumber >> _wavelenght1 >> _wavelenght2 >> _time1 >> _time2 >> _C1 >> _C2
                    >> _C3 >> _C4 >> _C5 >> _C6 >> _C7 >> _t >> _effects >> _formula >> _sort;


             LeaValues[row].formulaNumber=_formulaNumber;
             LeaValues[row].wavelenght1=_wavelenght1;
             LeaValues[row].wavelenght2=_wavelenght2;
             LeaValues[row].time1=_time1;
             LeaValues[row].time2=_time2;
             LeaValues[row].t=_t;
             LeaValues[row].effects=_effects;
             LeaValues[row].C1=_C1;
             LeaValues[row].C2=_C2;
             LeaValues[row].C3=_C3;
             LeaValues[row].C4=_C4;
             LeaValues[row].C5=_C5;
             LeaValues[row].C6=_C6;
             LeaValues[row].C7=_C7;
             LeaValues[row].formula=_formula;
             LeaValues[row].sort=_sort;
    }
     fileBinary.close();

     return LeaValues;
}

leadata* LeaTable::getLeaData_1_1M()
{
   return LeaValues_1_1M;
}

leadata* LeaTable::getLeaData_3R()
{
   return LeaValues_3R;
}

leadata* LeaTable::getLeaData_3B()
{
   return LeaValues_3B;
}
