#ifndef DOCKLEA_H
#define DOCKLEA_H

#include <QWidget>
#include "laserclasscw.h"
#include "laserclassmp.h"
#include "laserclasssp.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DockLea; }
QT_END_NAMESPACE

class DockLea : public QWidget
{
    Q_OBJECT

public:
    DockLea(QWidget *parent = nullptr);
    ~DockLea();
    void setDialControls();
    void setUVA();
    void setVIS();
    void setIRA();
    void setIRB();
    void setIRC();
    void setWidgets();
    void setWidgetsForOperation(LaserClassMP *);
    void setWidgetsForCW_Operation();
    void setWidgetsForSinglePulse_Operation();
    void setWidgetsForMultiPulse_Operation();
    void setWidgetsForThermal();
    void setWidgetsForThermalTi();
    void setPulsedWidgetsVisible(bool visible);

private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_wavelengthScrollBar_valueChanged(int value);
    void on_pulseControl_valueChanged();
    void on_operationCombo_currentIndexChanged(int index);
    void on_divergenceControl_valueChanged();
    void on_beamDiameterControl_valueChanged();
    void on_prfControl_valueChanged();
    void on_powerErgControl_valueChanged();
    void on_internalWaist_checkBox_toggled(bool checked);

private:
    Ui::DockLea *ui;
    ComputeLEA *myLaserLEA;
    double pulseWidth;
    double divergence;
    double alpha;
    double beamDiameter;
    double prf;
    double wavelength;
    double T_Skin;
    double powerErg;
    int n_laser;
    bool enableTeEditing;
    LaserClassMP* MyLaserClassMP;
    LaserClassCW* MyLaserCW_Pr;
    LaserClassSP* MyLaserSP_Pr;
    LaserClassMP* MyLaserMP_Pr;
};
#endif // DOCKLEA_H
