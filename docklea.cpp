#include "docklea.h"
#include "ui_docklea.h"

DockLea::DockLea(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DockLea)
{
    ui->setupUi(this);

    setVIS();

    prf=1.0e+001;
    beamDiameter=7.0;
    powerErg=1.0;
    divergence=1.5;
    wavelength=633;
    pulseWidth=0.25;
    alpha=1.5;

    ui->pulseControl->setEnabled(false);
    ui->prfControl->setEnabled(false);
    ui->wavelengthScrollBar->setValue(wavelength);
    ui->wavelengthLabel->setText(QString::number(wavelength));
    ui->comboBox->setCurrentIndex(1);

    n_laser=0;

    MyLaserCW_Pr=nullptr;
    MyLaserSP_Pr=nullptr;
    MyLaserMP_Pr=nullptr;

    //Nel costruttore: prf, beamDiameter, powerErg, divergence, wavelength, pulseWidth, alpha
    MyLaserClassMP=new LaserClassMP(prf, beamDiameter, powerErg, divergence, wavelength, pulseWidth, alpha);
    MyLaserCW_Pr=MyLaserClassMP;

    setDialControls();

    setWidgets();
}

DockLea::~DockLea()
{
    delete ui;
}

void DockLea::setDialControls()
{
    ui->powerErgControl->setTitle(tr("potenza [W]"));
    ui->powerErgControl->setMinimumExponent(-3);
    ui->powerErgControl->setMaximumExponent(6);
    ui->powerErgControl->setValue(powerErg);

    ui->pulseControl->setTitle(tr("t [s]"));
    ui->pulseControl->setMinimumExponent(-11);
    ui->pulseControl->setMaximumExponent(3);
    ui->pulseControl->setValue(pulseWidth);

    QString phi="φ";
    ui->divergenceControl->setTitle(phi.toHtmlEscaped() + " [mrad]");
    ui->divergenceControl->setMinimumExponent(-3);
    ui->divergenceControl->setMaximumExponent(2);
    ui->divergenceControl->setValue(divergence);

    ui->beamDiameterControl->setTitle(tr("a [mm]"));
    ui->beamDiameterControl->setMinimumExponent(-3);
    ui->beamDiameterControl->setMaximumExponent(2);
    ui->beamDiameterControl->setValue(beamDiameter);

    ui->prfControl->setTitle(tr("PRF [Hz]"));
    ui->prfControl->setMinimumExponent(0);
    ui->prfControl->setMaximumExponent(10);
    ui->prfControl->setValue(prf);
}

void DockLea::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1=="UVA")
        setUVA();
    else
    if(arg1=="VIS")
        setVIS();
    else
    if(arg1=="IRA")
        setIRA();
    else
    if(arg1=="IRB")
        setIRB();
    else
    if(arg1=="IRC")
        setIRC();
}

void DockLea::setUVA()
{
    ui->wavelengthScrollBar->setMinimum(180);
    ui->wavelengthScrollBar->setMaximum(399);
    ui->wavelengthScrollBar->setValue(380);
}

void DockLea::setVIS()
{
    ui->wavelengthScrollBar->setMinimum(400);
    ui->wavelengthScrollBar->setMaximum(700);
    ui->wavelengthScrollBar->setValue(633);
}

void DockLea::setIRA()
{
    ui->wavelengthScrollBar->setMinimum(701);
    ui->wavelengthScrollBar->setMaximum(1400);
    ui->wavelengthScrollBar->setValue(1064);
}

void DockLea::setIRB()
{
    ui->wavelengthScrollBar->setMinimum(1401);
    ui->wavelengthScrollBar->setMaximum(3000);
    ui->wavelengthScrollBar->setValue(1583);
}

void DockLea::setIRC()
{
    ui->wavelengthScrollBar->setMinimum(3001);
    ui->wavelengthScrollBar->setMaximum(20000);
    ui->wavelengthScrollBar->setValue(10600);
}

void DockLea::on_wavelengthScrollBar_valueChanged(int value)
{
    wavelength=(double)value;
    ui->wavelengthLabel->setText(QString::number(value));

    if(n_laser==0)
    {
    MyLaserCW_Pr->setTimeBase();
    MyLaserCW_Pr->setWavelength(wavelength);
    }
        else if(n_laser==1)
            MyLaserSP_Pr->setWavelength(wavelength);
        else if(n_laser==2){
                    MyLaserMP_Pr->setTimeBase();
                    MyLaserMP_Pr->setWavelength(wavelength);}

    setWidgets();
}

void DockLea::on_pulseControl_valueChanged()
{
    /********************************************
     * Ricavo il valore impostato dal controllo *
     ********************************************/
     int exponent=ui->pulseControl->getExponent();
     double mantissa=ui->pulseControl->getMantissa();

    /********************************************************************************+****
     * Memorizzo il valore impostato dal controllo nella variabile membro corrispondente *
     *************************************************************************************/
     pulseWidth=mantissa*powf(10, exponent);

     if(n_laser==1)
         MyLaserSP_Pr->setPulseWidth(pulseWidth);
     else if(n_laser==2)
         MyLaserMP_Pr->setPulseWidth(pulseWidth);

     setWidgets();
}

void DockLea::on_powerErgControl_valueChanged()
{
   /********************************************
    * Ricavo il valore impostato dal controllo *
    ********************************************/

    int exponent=ui->powerErgControl->getExponent();
    double mantissa=ui->powerErgControl->getMantissa();

   /********************************************************************************+****
    * Memorizzo il valore impostato dal controllo nella variabile membro corrispondente *
    *************************************************************************************/

    powerErg=mantissa*powf(10, exponent);

    if(n_laser==0)
    MyLaserCW_Pr->setPowerErg(powerErg);
        else if(n_laser==1)
        MyLaserSP_Pr->setPowerErg(powerErg);
            else if(n_laser==2)
            MyLaserMP_Pr->setPowerErg(powerErg);

    setWidgets();
}

void DockLea::on_prfControl_valueChanged()
{
   /********************************************
    * Ricavo il valore impostato dal controllo *
    ********************************************/

    int exponent=ui->prfControl->getExponent();
    double mantissa=ui->prfControl->getMantissa();

   /********************************************************************************+****
    * Memorizzo il valore impostato dal controllo nella variabile membro corrispondente *
    *************************************************************************************/

    prf=mantissa*powf(10, exponent);

    if(n_laser==2)
        MyLaserMP_Pr->setPRF(prf);

    setWidgets();
}

void DockLea::on_beamDiameterControl_valueChanged()
{
   /********************************************
    * Ricavo il valore impostato dal controllo *
    ********************************************/
    int exponent=ui->beamDiameterControl->getExponent();
    double mantissa=ui->beamDiameterControl->getMantissa();

   /********************************************************************************+****
    * Memorizzo il valore impostato dal controllo nella variabile membro corrispondente *
    *************************************************************************************/
    beamDiameter=mantissa*powf(10, exponent);

    if(n_laser==0)
        MyLaserCW_Pr->setBeamDiameter(beamDiameter);
        else if(n_laser==1)
            MyLaserSP_Pr->setBeamDiameter(beamDiameter);
                else if(n_laser==2)
                MyLaserMP_Pr->setBeamDiameter(beamDiameter);

    setWidgets();
}

void DockLea::on_divergenceControl_valueChanged()
{
    /********************************************
    * Ricavo il valore impostato dal controllo *
    ********************************************/
    int exponent=ui->divergenceControl->getExponent();
    double mantissa=ui->divergenceControl->getMantissa();

   /********************************************************************************+****
    * Memorizzo il valore impostato dal controllo nella variabile membro corrispondente *
    *************************************************************************************/
    divergence=mantissa*powf(10, exponent);

    if(n_laser==0)
    MyLaserCW_Pr->setDivergence(divergence);
        else if(n_laser==1)
        MyLaserSP_Pr->setDivergence(divergence);
            else if(n_laser==2)
            MyLaserMP_Pr->setDivergence(divergence);

    setWidgets();
}

void DockLea::on_operationCombo_currentIndexChanged(int index)
{
    /******************************************
    * L'indice scelto della casella combinata *
    *******************************************/
    n_laser=index;

   /******************
    * CONTINUOS WAVE *
    * ****************/
    if(n_laser==0)
    {

    /***********************************************************************************
    * Range di variazione della potenza nella modalità di funzionamento CONTINUOS WAVE *
    ************************************************************************************/

    ui->powerErgControl->setMinimumExponent(-1);
    ui->powerErgControl->setMaximumExponent(6);

   /*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
    MyLaserCW_Pr=MyLaserClassMP;

    ui->pulseControl->setEnabled(false);

    /*****************************************************************************
    * Ogni volta che passoa alla modalità di funzionamento CONTINUOS WAVE        *
    * Imposto il valore della potenza a 1.0e+00, il valore della frequenza a 0,  *
    * il tempo di esposizione della cute a 10                                    *
    ******************************************************************************/

    prf=0.0;//frequenza
    ui->prfControl->setValue(prf);
    ui->prfControl->setEnabled(false);
    //Non è possibile impostare il valore della prf essendo non definita per l'oggetto MyLaserCW_Pr


    powerErg=1.0e+00;//potenza
    ui->powerErgControl->setValue(powerErg);

    /****************************************************************************************
    * se l'impostazione del tempo di esposizione è abilitata aggiorno il laser con          *
    * il valore corrisposndente altrimenti aggiorno con il tempo di esposizione predefinito *
    *****************************************************************************************/

        MyLaserCW_Pr->setTimeBase();

    }
    else
    if (n_laser==1)
    {

   /************
    * IMPULSATO *
    * ***********/

   /****************************************************************
    * Range di variazione dell'energia dell'impulso nella modalità *
    * di funzionamento IMPULSATO                                   *
    ****************************************************************/

    ui->powerErgControl->setMinimumExponent(-12);
    ui->powerErgControl->setMaximumExponent(1);

   /*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
    MyLaserSP_Pr=MyLaserClassMP;

   /******************************************************************
    * Ogni volta che passo alla modalità di funzionamento IMPULSATO  *
    * imposto il valore dell'energia dell'impuslo a 1.0e+03, il      *
    * valore della frequenza a 0, il tempo di esposizione degli      *
    * occhi a 1.0e-06, il tempo di esposizione della cute a 1.0e-06. *
    ******************************************************************/

    prf=0;
    ui->prfControl->setValue(prf);
    ui->prfControl->setEnabled(false);

    pulseWidth=1.0e-06;
    ui->pulseControl->setEnabled(true);
    ui->pulseControl->setValue(pulseWidth);

    powerErg=1.0e-03;
    ui->powerErgControl->setEnabled(true);
    ui->powerErgControl->setValue(powerErg);
    }
    else
   /********************
    * IMPULSI RIPETUTI *
    * ******************/
    if (n_laser==2)
    {

   /****************************************************************
    * Range di variazione dell'energia dell'impulso nella modalità *
    * di funzionamento IMPULSI RIPETUTI.                            *
    ****************************************************************/

    ui->powerErgControl->setMinimumExponent(-12);
    ui->powerErgControl->setMaximumExponent(1);

   /*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
    MyLaserMP_Pr=MyLaserClassMP;

   /******************************************************************
    * Ogni volta che passo alla modalità di funzionamento AD IMPULSI *
    * RIPETUTI imposto il valore dell'energia dell'impusl a 1.0e+03, il valore della     *
    * frequenza a 10.0, il tempo di esposizione degli occhi a 1.0e-06,  *
    * il tempo di esposizione della cute a 10. 				 *
    ******************************************************************/
    powerErg=1.0e-03;
    MyLaserMP_Pr->setPowerErg(powerErg);
    ui->powerErgControl->setValue(powerErg);

    //ogni volta cha passo al funzionamento ad impulsi ripetuti imposto la durata degli impulsi a 1.0e-06 s
    pulseWidth=1.0e-06;
    ui->pulseControl->setEnabled(true);
    ui->pulseControl->setValue(pulseWidth);

    //ogni volta cha passo al funzionamento ad impulsi ripetuti imposto la frequenza a 10 Hz
    prf=10;
    ui->prfControl->setEnabled(true);
    ui->prfControl->setValue(prf);
    MyLaserMP_Pr->setPRF(prf);

    //Imposto il tempo di esposizione alla radiazione laser.
    MyLaserMP_Pr->setTimeBase();
    }

   /*******************************************
    * Imposto i widget per la visualizzazione *
    *******************************************/
    setWidgets();
}


void DockLea::setWidgets()
{
    if(((wavelength>=400)and(wavelength<=700)))
       {
        ui->frame_2M->setVisible(true);
        ui->frame_2M_2->setVisible(true);
        ui->frame_2M_3->setVisible(true);
        ui->frame_2M_Output->setVisible(true);
        ui->frame_2M_Output_2->setVisible(true);
        ui->frame_2M_Output_3->setVisible(true);}
        else
            {
            ui->frame_2M->setVisible(false);
            ui->frame_2M_2->setVisible(false);
            ui->frame_2M_3->setVisible(false);
            ui->frame_2M_Output->setVisible(false);
            ui->frame_2M_Output_2->setVisible(false);
            ui->frame_2M_Output_3->setVisible(false);
            }

        if(((wavelength<302.5)or(wavelength>4000)))
        {
                ui->frame_cond1->setVisible(false);
                ui->frame_cond1_1->setVisible(false);
                ui->frame_cond1_2->setVisible(false);
                ui->tBeamAperture1_Label->setVisible(false);
                ui->beamAperture1_Label->setVisible(false);
                ui->tBeamAperture1_Label_2->setVisible(false);
                ui->beamAperture1_Label_2->setVisible(false);
                ui->tBeamAperture1_Label_3->setVisible(false);
                ui->beamAperture1_Label_3->setVisible(false);
        }
            else
            {
                ui->frame_cond1->setVisible(true);
                ui->frame_cond1_1->setVisible(true);
                ui->frame_cond1_2->setVisible(true);
                ui->tBeamAperture1_Label->setVisible(true);
                ui->beamAperture1_Label->setVisible(true);
                ui->tBeamAperture1_Label_2->setVisible(true);
                ui->beamAperture1_Label_2->setVisible(true);
                ui->tBeamAperture1_Label_3->setVisible(true);
                ui->beamAperture1_Label_3->setVisible(true);
            }


    if(n_laser==0)
    {
        MyLaserCW_Pr->updateAll();
        setWidgetsForCW_Operation();
        ui->frame_base->setVisible(true);
        ui->frame_meanPower->setVisible(false);
        ui->frame_thermal->setVisible(false);
        ui->base_Label->setText("Valutazione per laser CW");
        ui->frame_pulses->setVisible(false);
        ui->frame_class2->setVisible(false);
        ui->frame_class3->setVisible(false);
        ui->frame_class4->setVisible(false);
    }
    else if(n_laser==1)
    {
        MyLaserSP_Pr->updateAll();
        setWidgetsForSinglePulse_Operation();
        ui->frame_base->setVisible(true);
        ui->frame_meanPower->setVisible(false);
        ui->frame_thermal->setVisible(false);
        ui->frame_pulses->setVisible(false);
        ui->base_Label->setText("Valutazione per laser ad impulso singolo");
        ui->frame_class2->setVisible(false);
        ui->frame_class3->setVisible(false);
        ui->frame_class4->setVisible(false);
    }
    else if(n_laser==2)
    {
        MyLaserMP_Pr->updateAll();
        setWidgetsForMultiPulse_Operation();
        ui->frame_base->setVisible(true);
        ui->frame_meanPower->setVisible(true);
        ui->frame_pulses->setVisible(true);
        ui->base_Label->setText("Valutazione per laser ad impulsi multipli - impulso singolo");
        ui->meanPower_Label->setText("Valutazione effetti medi");
        ui->frame_class2->setVisible(true);
        ui->frame_class4->setVisible(true);

        if((wavelength>=400)and(wavelength<=1400))
        {
         ui->frame_class3->setVisible(true);
         ui->frame_thermal->setVisible(true);
         ui->frame_Ti_prf->setVisible(true);;

            if(prf<1/MyLaserMP_Pr->getTi())
            {
                setWidgetsForThermal();
                ui->thermal_label->setText("Valutazione effetti termici");
            }
            else
            {
                setWidgetsForThermalTi();
                ui->thermal_label->setText("Valutazione effetti termici ad alta prequenza di ripetizione");
            }
        }
        else{
                ui->frame_thermal->setVisible(false);       
                ui->frame_class3->setVisible(false);
                ui->frame_Ti_prf->setVisible(false);
        }
    }
}

void DockLea:: setPulsedWidgetsVisible(bool visible)
{
    ui->frame_thermal->setVisible(visible);
    ui->frame_meanPower->setVisible(visible);
}

void DockLea::setWidgetsForCW_Operation()
{
    ui->powerErgControl->setTitle("Potenza [W]");
    ui->C1_Label->setText(QString::number(MyLaserCW_Pr->getC1(), 'e', 2));
    ui->C2_Label->setText(QString::number(MyLaserCW_Pr->getC2(), 'e', 2));
    ui->C3_Label->setText(QString::number(MyLaserCW_Pr->getC3(), 'e', 2));
    ui->C4_Label->setText(QString::number(MyLaserCW_Pr->getC4(), 'e', 2));
    ui->C5_Label->setText(QString::number(MyLaserCW_Pr->getC5(), 'e', 2));
    ui->C6_Label->setText(QString::number(MyLaserCW_Pr->getC6(), 'e', 2));
    ui->C7_Label->setText(QString::number(MyLaserCW_Pr->getC7(), 'e', 2));
    ui->T1_Label->setText(QString::number(MyLaserCW_Pr->getT1(), 'e', 2)+" s");
    ui->T2_Label->setText(QString::number(MyLaserCW_Pr->getT2(), 'e', 2)+" s");
    ui->couplingFactor1_Label->setText(QString::number(MyLaserCW_Pr->getCouplingFactor_Cond_1(), 'e', 2));
    ui->couplingFactor3_Label->setText(QString::number(MyLaserCW_Pr->getCouplingFactor_Cond_3(), 'e', 2));
    ui->apertureDiam1_Label->setText(QString::number(MyLaserCW_Pr->getApCond_1(), 'e', 2)+" mm");
    ui->apertureDiam3_Label->setText(QString::number(MyLaserCW_Pr->getApCond_3(), 'e', 2)+" mm");
    ui->apertureDist1_Label->setText(QString::number(MyLaserCW_Pr->getDistCond_1(), 'e', 2)+" mm");
    ui->apertureDist3_Label->setText(QString::number(MyLaserCW_Pr->getDistCond_3(), 'e', 2)+" mm");
    ui->beamAperture1_Label->setText(QString::number(MyLaserCW_Pr->getBeamAtStop_Cond_1(), 'e', 2)+" mm");
    ui->beamAperture3_Label->setText(QString::number(MyLaserCW_Pr->getBeamAtStop_Cond_3(), 'e', 2)+" mm");

    QString TimeBase_Label=QString("%1 s")
                         .arg(QString::number(MyLaserCW_Pr->getTimeBase()));

    ui->timeBase_Label->setText(TimeBase_Label);


    QString Formula1M_Tipo=QString::fromStdString(MyLaserCW_Pr->getLEA_FormulaTipo()[0]);
    QString Formula1M=QString::fromStdString(MyLaserCW_Pr->getLEA_Formula()[0]);
    QString Formula1M_Unit=QString::fromStdString(MyLaserCW_Pr->getLEA_FormulaUnit()[0]);
    QString LEA_1M=QString::number(MyLaserCW_Pr->getLEA()[0], 'e', 2);
    QString Formula1M_Label=QString("%1=%2 %3")
                         .arg(Formula1M_Tipo)
                         .arg(Formula1M)
                         .arg(Formula1M_Unit);

    QString LEA_1M_Label=QString("%1=%2 %3")
                         .arg(Formula1M_Tipo)
                         .arg(LEA_1M)
                         .arg(Formula1M_Unit);

    ui->Formula1M_Label->setText(Formula1M_Label);
    ui->LEA1M_label->setText(LEA_1M_Label);

    QString PowerErg1M_Cond_1=QString::number(MyLaserCW_Pr->getPowerErg_Cond_1()[0], 'e', 2);
    QString PowerErg1M_Cond_3=QString::number(MyLaserCW_Pr->getPowerErg_Cond_3()[0], 'e', 2);

    QString PowerErg1M_Cond_1_Label=QString("%1=%2 %3")
                         .arg(Formula1M_Tipo)
                         .arg(PowerErg1M_Cond_1)
                         .arg(Formula1M_Unit);

    QString PowerErg1M_Cond_3_Label=QString("%1=%2 %3")
                         .arg(Formula1M_Tipo)
                         .arg(PowerErg1M_Cond_3)
                         .arg(Formula1M_Unit);

    ui->cond1LEA_1M_Label->setText(PowerErg1M_Cond_1_Label);
    ui->cond3LEA_1M_Label->setText(PowerErg1M_Cond_3_Label);

    QString tPowerErg1M_Cond_1_Label=QString("%1<sub>Acc 1</sub>").arg(Formula1M_Tipo);
    ui->tCond1LEA_1M_Label->setText(tPowerErg1M_Cond_1_Label);

    QString tPowerErg1M_Cond_3_Label=QString("%1<sub>Acc 3</sub>").arg(Formula1M_Tipo);
    ui->tCond3LEA_1M_Label->setText(tPowerErg1M_Cond_3_Label);

    QString Formula2M_Tipo=QString::fromStdString(MyLaserCW_Pr->getLEA_FormulaTipo()[1]);
    QString Formula2M=QString::fromStdString(MyLaserCW_Pr->getLEA_Formula()[1]);
    QString Formula2M_Unit=QString::fromStdString(MyLaserCW_Pr->getLEA_FormulaUnit()[1]);
    QString LEA_2M=QString::number(MyLaserCW_Pr->getLEA()[1], 'e', 2);

    QString Formula2M_Label;
    QString LEA_2M_Label;

    Formula2M_Label=QString("%1=%2 %3")
                           .arg(Formula2M_Tipo)
                           .arg(Formula2M)
                           .arg(Formula2M_Unit);

    LEA_2M_Label=QString("%1=%2 %3")
                           .arg(Formula2M_Tipo)
                           .arg(LEA_2M)
                           .arg(Formula2M_Unit);


    ui->Formula2M_Label->setText(Formula2M_Label);
    ui->LEA2M_label->setText(LEA_2M_Label);

    QString PowerErg2M_Cond_1=QString::number(MyLaserCW_Pr->getPowerErg_Cond_1()[1], 'e', 2);
    QString PowerErg2M_Cond_3=QString::number(MyLaserCW_Pr->getPowerErg_Cond_3()[1], 'e', 2);

    QString PowerErg2M_Cond_1_Label=QString("%1=%2 %3")
                         .arg(Formula2M_Tipo)
                         .arg(PowerErg2M_Cond_1)
                         .arg(Formula2M_Unit);

    QString PowerErg2M_Cond_3_Label=QString("%1=%2 %3")
                         .arg(Formula2M_Tipo)
                         .arg(PowerErg2M_Cond_3)
                         .arg(Formula2M_Unit);

    ui->cond1LEA_2M_Label->setText(PowerErg2M_Cond_1_Label);
    ui->cond3LEA_2M_Label->setText(PowerErg2M_Cond_3_Label);

    QString tPowerErg2M_Cond_1_Label=QString("%1<sub>Acc 1</sub>").arg(Formula2M_Tipo);
    ui->tCond1LEA_2M_Label->setText(tPowerErg2M_Cond_1_Label);

    QString tPowerErg2M_Cond_3_Label=QString("%1<sub>Acc 3</sub>").arg(Formula2M_Tipo);
    ui->tCond3LEA_2M_Label->setText(tPowerErg2M_Cond_3_Label);

    QString Formula3R_Tipo=QString::fromStdString(MyLaserCW_Pr->getLEA_FormulaTipo()[2]);
    QString Formula3R=QString::fromStdString(MyLaserCW_Pr->getLEA_Formula()[2]);
    QString Formula3R_Unit=QString::fromStdString(MyLaserCW_Pr->getLEA_FormulaUnit()[2]);
    QString LEA_3R=QString::number(MyLaserCW_Pr->getLEA()[2], 'e', 2);
    QString Formula3R_Label=QString("%1=%2 %3")
                           .arg(Formula3R_Tipo)
                           .arg(Formula3R)
                           .arg(Formula3R_Unit);

    QString LEA_3R_Label=QString("%1=%2 %3")
                            .arg(Formula3R_Tipo)
                            .arg(LEA_3R)
                            .arg(Formula3R_Unit);


    ui->Formula3R_Label->setText(Formula3R_Label);
    ui->LEA3R_label->setText(LEA_3R_Label);

    QString PowerErg3R_Cond_1=QString::number(MyLaserCW_Pr->getPowerErg_Cond_1()[2], 'e', 2);
    QString PowerErg3R_Cond_3=QString::number(MyLaserCW_Pr->getPowerErg_Cond_3()[2], 'e', 2);

    QString PowerErg3R_Cond_1_Label=QString("%1=%2 %3")
                         .arg(Formula3R_Tipo)
                         .arg(PowerErg3R_Cond_1)
                         .arg(Formula3R_Unit);

    QString PowerErg3R_Cond_3_Label=QString("%1=%2 %3")
                         .arg(Formula3R_Tipo)
                         .arg(PowerErg3R_Cond_3)
                         .arg(Formula3R_Unit);

    ui->cond1LEA_3R_Label->setText(PowerErg3R_Cond_1_Label);
    ui->cond3LEA_3R_Label->setText(PowerErg3R_Cond_3_Label);

    QString tPowerErg3R_Cond_1_Label=QString("%1<sub>Acc 1</sub>").arg(Formula3R_Tipo);
    ui->tCond1LEA_3R_Label->setText(tPowerErg3R_Cond_1_Label);

    QString tPowerErg3R_Cond_3_Label=QString("%1<sub>Acc 3</sub>").arg(Formula3R_Tipo);
    ui->tCond3LEA_3R_Label->setText(tPowerErg3R_Cond_3_Label);

    QString Formula3B_Tipo=QString::fromStdString(MyLaserCW_Pr->getLEA_FormulaTipo()[3]);
    QString Formula3B=QString::fromStdString(MyLaserCW_Pr->getLEA_Formula()[3]);
    QString Formula3B_Unit=QString::fromStdString(MyLaserCW_Pr->getLEA_FormulaUnit()[3]);
    QString LEA_3B=QString::number(MyLaserCW_Pr->getLEA()[3], 'e', 2);
    QString Formula3B_Label=QString("%1=%2 %3")
                            .arg(Formula3B_Tipo)
                            .arg(Formula3B)
                            .arg(Formula3B_Unit);

    QString LEA_3B_Label=QString("%1=%2 %3")
                            .arg(Formula3B_Tipo)
                            .arg(LEA_3B)
                            .arg(Formula3R_Unit);

    ui->Formula3B_Label->setText(Formula3B_Label);
    ui->LEA3B_label->setText(LEA_3B_Label);

    QString PowerErg3B_Cond_1=QString::number(MyLaserCW_Pr->getPowerErg_Cond_1()[3], 'e', 2);
    QString PowerErg3B_Cond_3=QString::number(MyLaserCW_Pr->getPowerErg_Cond_3()[3], 'e', 2);

    QString PowerErg3B_Cond_1_Label=QString("%1=%2 %3")
                         .arg(Formula3B_Tipo)
                         .arg(PowerErg3B_Cond_1)
                         .arg(Formula3B_Unit);

    QString PowerErg3B_Cond_3_Label=QString("%1=%2 %3")
                         .arg(Formula3B_Tipo)
                         .arg(PowerErg3B_Cond_3)
                         .arg(Formula3B_Unit);

    ui->cond1LEA_3B_Label->setText(PowerErg3B_Cond_1_Label);
    ui->cond3LEA_3B_Label->setText(PowerErg3B_Cond_3_Label);

    QString tPowerErg3B_Cond_1_Label=QString("%1<sub>Acc 1</sub>").arg(Formula3B_Tipo);
    ui->tCond1LEA_3B_Label->setText(tPowerErg3B_Cond_1_Label);

    QString tPowerErg3B_Cond_3_Label=QString("%1<sub>Acc 3</sub>").arg(Formula3B_Tipo);
    ui->tCond3LEA_3B_Label->setText(tPowerErg3B_Cond_3_Label);

    ui->SkinDamage_Label->setText(QString::fromStdString(MyLaserCW_Pr->getSkinDamage()));
    ui->EyeDamage_Label->setText(QString::fromStdString(MyLaserCW_Pr->getEyeDamage()));
    ui->Radiation_Label->setText(QString::fromStdString(MyLaserCW_Pr->getRadiation()));
    ui->class_Label->setText(QString::fromStdString(MyLaserCW_Pr->getLaserClass()));
}

void DockLea::setWidgetsForSinglePulse_Operation()
{
    ui->powerErgControl->setTitle("Energia [J]");
    ui->C1_Label->setText(QString::number(MyLaserSP_Pr->getC1(), 'e', 2));
    ui->C2_Label->setText(QString::number(MyLaserSP_Pr->getC2(), 'e', 2));
    ui->C3_Label->setText(QString::number(MyLaserSP_Pr->getC3(), 'e', 2));
    ui->C4_Label->setText(QString::number(MyLaserSP_Pr->getC4(), 'e', 2));
    ui->C5_Label->setText(QString::number(MyLaserSP_Pr->getC5(), 'e', 2));
    ui->C6_Label->setText(QString::number(MyLaserSP_Pr->getC6(), 'e', 2));
    ui->C7_Label->setText(QString::number(MyLaserSP_Pr->getC7(), 'e', 2));
    ui->T1_Label->setText(QString::number(MyLaserSP_Pr->getT1(), 'e', 2)+" s");
    ui->T2_Label->setText(QString::number(MyLaserSP_Pr->getT2(), 'e', 2)+" s");
    ui->couplingFactor1_Label->setText(QString::number(MyLaserSP_Pr->getCouplingFactor_Cond_1(), 'e', 2));
    ui->couplingFactor3_Label->setText(QString::number(MyLaserSP_Pr->getCouplingFactor_Cond_3(), 'e', 2));
    ui->apertureDiam1_Label->setText(QString::number(MyLaserSP_Pr->getApCond_1(), 'e', 2)+" mm");
    ui->apertureDiam3_Label->setText(QString::number(MyLaserSP_Pr->getApCond_3(), 'e', 2)+" mm");
    ui->apertureDist1_Label->setText(QString::number(MyLaserSP_Pr->getDistCond_1(), 'e', 2)+" mm");
    ui->apertureDist3_Label->setText(QString::number(MyLaserSP_Pr->getDistCond_3(), 'e', 2)+" mm");
    ui->beamAperture1_Label->setText(QString::number(MyLaserSP_Pr->getBeamAtStop_Cond_1(), 'e', 2)+" mm");
    ui->beamAperture3_Label->setText(QString::number(MyLaserSP_Pr->getBeamAtStop_Cond_3(), 'e', 2)+" mm");

    ui->timeBase_Label->setText("N.A.");

    QString Formula1M_Tipo=QString::fromStdString(MyLaserSP_Pr->getLEA_FormulaTipo()[0]);
    QString Formula1M=QString::fromStdString(MyLaserSP_Pr->getLEA_Formula()[0]);
    QString Formula1M_Unit=QString::fromStdString(MyLaserSP_Pr->getLEA_FormulaUnit()[0]);
    QString LEA_1M=QString::number(MyLaserSP_Pr->getLEA()[0], 'e', 2);
    QString Formula1M_Label=QString("%1=%2 %3")
                         .arg(Formula1M_Tipo)
                         .arg(Formula1M)
                         .arg(Formula1M_Unit);

    QString LEA_1M_Label=QString("%1=%2 %3")
                         .arg(Formula1M_Tipo)
                         .arg(LEA_1M)
                         .arg(Formula1M_Unit);

    ui->Formula1M_Label->setText(Formula1M_Label);
    ui->LEA1M_label->setText(LEA_1M_Label);

    QString PowerErg1M_Cond_1=QString::number(MyLaserSP_Pr->getPowerErg_Cond_1()[0], 'e', 2);
    QString PowerErg1M_Cond_3=QString::number(MyLaserSP_Pr->getPowerErg_Cond_3()[0], 'e', 2);

    QString PowerErg1M_Cond_1_Label=QString("%1=%2 %3")
                         .arg(Formula1M_Tipo)
                         .arg(PowerErg1M_Cond_1)
                         .arg(Formula1M_Unit);

    QString PowerErg1M_Cond_3_Label=QString("%1=%2 %3")
                         .arg(Formula1M_Tipo)
                         .arg(PowerErg1M_Cond_3)
                         .arg(Formula1M_Unit);

    ui->cond1LEA_1M_Label->setText(PowerErg1M_Cond_1_Label);
    ui->cond3LEA_1M_Label->setText(PowerErg1M_Cond_3_Label);

    QString tPowerErg1M_Cond_1_Label=QString("%1<sub>Acc 1</sub>").arg(Formula1M_Tipo);
    ui->tCond1LEA_1M_Label->setText(tPowerErg1M_Cond_1_Label);

    QString tPowerErg1M_Cond_3_Label=QString("%1<sub>Acc 3</sub>").arg(Formula1M_Tipo);
    ui->tCond3LEA_1M_Label->setText(tPowerErg1M_Cond_3_Label);

    QString Formula2M_Tipo=QString::fromStdString(MyLaserSP_Pr->getLEA_FormulaTipo()[1]);
    QString Formula2M=QString::fromStdString(MyLaserSP_Pr->getLEA_Formula()[1]);
    QString Formula2M_Unit=QString::fromStdString(MyLaserSP_Pr->getLEA_FormulaUnit()[1]);
    QString LEA_2M=QString::number(MyLaserSP_Pr->getLEA()[1], 'e', 2);

    QString Formula2M_Label;
    QString LEA_2M_Label;

    Formula2M_Label=QString("%1=%2 %3")
                           .arg(Formula2M_Tipo)
                           .arg(Formula2M)
                           .arg(Formula2M_Unit);

    LEA_2M_Label=QString("%1=%2 %3")
                           .arg(Formula2M_Tipo)
                           .arg(LEA_2M)
                           .arg(Formula2M_Unit);

    ui->Formula2M_Label->setText(Formula2M_Label);
    ui->LEA2M_label->setText(LEA_2M_Label);

    QString PowerErg2M_Cond_1=QString::number(MyLaserSP_Pr->getPowerErg_Cond_1()[1], 'e', 2);
    QString PowerErg2M_Cond_3=QString::number(MyLaserSP_Pr->getPowerErg_Cond_3()[1], 'e', 2);

    QString PowerErg2M_Cond_1_Label=QString("%1=%2 %3")
                         .arg(Formula2M_Tipo)
                         .arg(PowerErg2M_Cond_1)
                         .arg(Formula2M_Unit);

    QString PowerErg2M_Cond_3_Label=QString("%1=%2 %3")
                         .arg(Formula2M_Tipo)
                         .arg(PowerErg2M_Cond_3)
                         .arg(Formula2M_Unit);

    ui->cond1LEA_2M_Label->setText(PowerErg2M_Cond_1_Label);
    ui->cond3LEA_2M_Label->setText(PowerErg2M_Cond_3_Label);

    QString tPowerErg2M_Cond_1_Label=QString("%1<sub>Acc 1</sub>").arg(Formula2M_Tipo);
    ui->tCond1LEA_2M_Label->setText(tPowerErg2M_Cond_1_Label);

    QString tPowerErg2M_Cond_3_Label=QString("%1<sub>Acc 3</sub>").arg(Formula2M_Tipo);
    ui->tCond3LEA_2M_Label->setText(tPowerErg2M_Cond_3_Label);

    QString Formula3R_Tipo=QString::fromStdString(MyLaserSP_Pr->getLEA_FormulaTipo()[2]);
    QString Formula3R=QString::fromStdString(MyLaserSP_Pr->getLEA_Formula()[2]);
    QString Formula3R_Unit=QString::fromStdString(MyLaserSP_Pr->getLEA_FormulaUnit()[2]);
    QString LEA_3R=QString::number(MyLaserSP_Pr->getLEA()[2], 'e', 2);
    QString Formula3R_Label=QString("%1=%2 %3")
                           .arg(Formula3R_Tipo)
                           .arg(Formula3R)
                           .arg(Formula3R_Unit);

    QString LEA_3R_Label=QString("%1=%2 %3")
                            .arg(Formula3R_Tipo)
                            .arg(LEA_3R)
                            .arg(Formula3R_Unit);


    ui->Formula3R_Label->setText(Formula3R_Label);
    ui->LEA3R_label->setText(LEA_3R_Label);

    QString PowerErg3R_Cond_1=QString::number(MyLaserSP_Pr->getPowerErg_Cond_1()[2], 'e', 2);
    QString PowerErg3R_Cond_3=QString::number(MyLaserSP_Pr->getPowerErg_Cond_3()[2], 'e', 2);

    QString PowerErg3R_Cond_1_Label=QString("%1=%2 %3")
                         .arg(Formula3R_Tipo)
                         .arg(PowerErg3R_Cond_1)
                         .arg(Formula3R_Unit);

    QString PowerErg3R_Cond_3_Label=QString("%1=%2 %3")
                         .arg(Formula3R_Tipo)
                         .arg(PowerErg3R_Cond_3)
                         .arg(Formula3R_Unit);

    ui->cond1LEA_3R_Label->setText(PowerErg3R_Cond_1_Label);
    ui->cond3LEA_3R_Label->setText(PowerErg3R_Cond_3_Label);

    QString tPowerErg3R_Cond_1_Label=QString("%1<sub>Acc 1</sub>").arg(Formula3R_Tipo);
    ui->tCond1LEA_3R_Label->setText(tPowerErg3R_Cond_1_Label);

    QString tPowerErg3R_Cond_3_Label=QString("%1<sub>Acc 3</sub>").arg(Formula3R_Tipo);
    ui->tCond3LEA_3R_Label->setText(tPowerErg3R_Cond_3_Label);

    QString Formula3B_Tipo=QString::fromStdString(MyLaserSP_Pr->getLEA_FormulaTipo()[3]);
    QString Formula3B=QString::fromStdString(MyLaserSP_Pr->getLEA_Formula()[3]);
    QString Formula3B_Unit=QString::fromStdString(MyLaserSP_Pr->getLEA_FormulaUnit()[3]);
    QString LEA_3B=QString::number(MyLaserSP_Pr->getLEA()[3], 'e', 2);
    QString Formula3B_Label=QString("%1=%2 %3")
                            .arg(Formula3B_Tipo)
                            .arg(Formula3B)
                            .arg(Formula3B_Unit);

    QString LEA_3B_Label=QString("%1=%2 %3")
                            .arg(Formula3B_Tipo)
                            .arg(LEA_3B)
                            .arg(Formula3R_Unit);

    ui->Formula3B_Label->setText(Formula3B_Label);
    ui->LEA3B_label->setText(LEA_3B_Label);

    QString PowerErg3B_Cond_1=QString::number(MyLaserSP_Pr->getPowerErg_Cond_1()[3], 'e', 2);
    QString PowerErg3B_Cond_3=QString::number(MyLaserSP_Pr->getPowerErg_Cond_3()[3], 'e', 2);

    QString PowerErg3B_Cond_1_Label=QString("%1=%2 %3")
                         .arg(Formula3B_Tipo)
                         .arg(PowerErg3B_Cond_1)
                         .arg(Formula3B_Unit);

    QString PowerErg3B_Cond_3_Label=QString("%1=%2 %3")
                         .arg(Formula3B_Tipo)
                         .arg(PowerErg3B_Cond_3)
                         .arg(Formula3B_Unit);

    ui->cond1LEA_3B_Label->setText(PowerErg3B_Cond_1_Label);
    ui->cond3LEA_3B_Label->setText(PowerErg3B_Cond_3_Label);

    QString tPowerErg3B_Cond_1_Label=QString("%1<sub>Acc 1</sub>").arg(Formula3B_Tipo);
    ui->tCond1LEA_3B_Label->setText(tPowerErg3B_Cond_1_Label);

    QString tPowerErg3B_Cond_3_Label=QString("%1<sub>Acc 3</sub>").arg(Formula3B_Tipo);
    ui->tCond3LEA_3B_Label->setText(tPowerErg3B_Cond_3_Label);

    ui->SkinDamage_Label->setText(QString::fromStdString(MyLaserSP_Pr->getSkinDamage()));
    ui->EyeDamage_Label->setText(QString::fromStdString(MyLaserSP_Pr->getEyeDamage()));
    ui->Radiation_Label->setText(QString::fromStdString(MyLaserSP_Pr->getRadiation()));
    ui->class_Label->setText(QString::fromStdString(MyLaserSP_Pr->getLaserClass()));
}

void DockLea::setWidgetsForMultiPulse_Operation()
{
    ui->powerErgControl->setTitle("Energia [J]");
    ui->C1_Label->setText(QString::number(MyLaserMP_Pr->getC1(), 'e', 2));
    ui->C2_Label->setText(QString::number(MyLaserMP_Pr->getC2(), 'e', 2));
    ui->C3_Label->setText(QString::number(MyLaserMP_Pr->getC3(), 'e', 2));
    ui->C4_Label->setText(QString::number(MyLaserMP_Pr->getC4(), 'e', 2));
    ui->C5_Label->setText(QString::number(MyLaserMP_Pr->getC5(), 'e', 2));
    ui->C6_Label->setText(QString::number(MyLaserMP_Pr->getC6(), 'e', 2));
    ui->C7_Label->setText(QString::number(MyLaserMP_Pr->getC7(), 'e', 2));
    ui->T1_Label->setText(QString::number(MyLaserMP_Pr->getT1(), 'e', 2)+" s");
    ui->T2_Label->setText(QString::number(MyLaserMP_Pr->getT2(), 'e', 2)+" s");
    ui->Te_Label->setText(QString::number(MyLaserMP_Pr->getTe(), 'e', 2)+" s");
    ui->Ti_Label->setText(QString::number(MyLaserMP_Pr->getTi(), 'e', 2)+" s");
    ui->class_Label_4->setText(QString::fromStdString(MyLaserMP_Pr->getSystemClassValutation()));

    ui->couplingFactor1_Label->setText(QString::number(MyLaserMP_Pr->getCouplingFactor_Cond_1(), 'e', 2));
    ui->couplingFactor3_Label->setText(QString::number(MyLaserMP_Pr->getCouplingFactor_Cond_3(), 'e', 2));
    ui->apertureDiam1_Label->setText(QString::number(MyLaserMP_Pr->getApCond_1(), 'e', 2)+" mm");
    ui->apertureDiam3_Label->setText(QString::number(MyLaserMP_Pr->getApCond_3(), 'e', 2)+" mm");
    ui->apertureDist1_Label->setText(QString::number(MyLaserMP_Pr->getDistCond_1(), 'e', 2)+" mm");
    ui->apertureDist3_Label->setText(QString::number(MyLaserMP_Pr->getDistCond_3(), 'e', 2)+" mm");
    ui->beamAperture1_Label->setText(QString::number(MyLaserMP_Pr->getBeamAtStop_Cond_1(), 'e', 2)+" mm");
    ui->beamAperture3_Label->setText(QString::number(MyLaserMP_Pr->getBeamAtStop_Cond_3(), 'e', 2)+" mm");
    ui->Ti_prf_Label->setText(QString::number(MyLaserMP_Pr->getTi()*MyLaserMP_Pr->getPRF()));

    QString TimeBase_Label=QString("%1 s")
                         .arg(QString::number(MyLaserMP_Pr->getTimeBase()));


    QString Formula1M_Tipo=QString::fromStdString(MyLaserMP_Pr->getLEA_FormulaTipo()[0]);
    QString Formula1M=QString::fromStdString(MyLaserMP_Pr->getLEA_Formula()[0]);
    QString Formula1M_Unit=QString::fromStdString(MyLaserMP_Pr->getLEA_FormulaUnit()[0]);
    QString LEA_1M=QString::number(MyLaserMP_Pr->getLEA()[0], 'e', 2);
    QString Formula1M_Label=QString("%1=%2 %3")
                         .arg(Formula1M_Tipo)
                         .arg(Formula1M)
                         .arg(Formula1M_Unit);
         QString LEA_1M_Label=QString("%1=%2 %3")
                         .arg(Formula1M_Tipo)
                         .arg(LEA_1M)
                         .arg(Formula1M_Unit);
         ui->Formula1M_Label->setText(Formula1M_Label);
    ui->LEA1M_label->setText(LEA_1M_Label);
    QString PowerErg1M_Cond_1=QString::number(MyLaserMP_Pr->getPowerErg_Cond_1()[0], 'e', 2);
    QString PowerErg1M_Cond_3=QString::number(MyLaserMP_Pr->getPowerErg_Cond_3()[0], 'e', 2);
    QString PowerErg1M_Cond_1_Label=QString("%1=%2 %3")
                         .arg(Formula1M_Tipo)
                         .arg(PowerErg1M_Cond_1)
                         .arg(Formula1M_Unit);
    QString PowerErg1M_Cond_3_Label=QString("%1=%2 %3")
                         .arg(Formula1M_Tipo)
                         .arg(PowerErg1M_Cond_3)
                         .arg(Formula1M_Unit);
    ui->cond1LEA_1M_Label->setText(PowerErg1M_Cond_1_Label);
    ui->cond3LEA_1M_Label->setText(PowerErg1M_Cond_3_Label);

    QString tPowerErg1M_Cond_1_Label=QString("%1<sub>Acc 1</sub>").arg(Formula1M_Tipo);
    ui->tCond1LEA_1M_Label->setText(tPowerErg1M_Cond_1_Label);

    QString tPowerErg1M_Cond_3_Label=QString("%1<sub>Acc 3</sub>").arg(Formula1M_Tipo);
    ui->tCond3LEA_1M_Label->setText(tPowerErg1M_Cond_3_Label);

    QString Formula2M_Tipo=QString::fromStdString(MyLaserMP_Pr->getLEA_FormulaTipo()[1]);
    QString Formula2M=QString::fromStdString(MyLaserMP_Pr->getLEA_Formula()[1]);
    QString Formula2M_Unit=QString::fromStdString(MyLaserMP_Pr->getLEA_FormulaUnit()[1]);
    QString LEA_2M=QString::number(MyLaserMP_Pr->getLEA()[1], 'e', 2);

    QString Formula2M_Label;
    QString LEA_2M_Label;

    Formula2M_Label=QString("%1=%2 %3")
                           .arg(Formula2M_Tipo)
                           .arg(Formula2M)
                           .arg(Formula2M_Unit);

    LEA_2M_Label=QString("%1=%2 %3")
                           .arg(Formula2M_Tipo)
                           .arg(LEA_2M)
                           .arg(Formula2M_Unit);

    ui->Formula2M_Label->setText(Formula2M_Label);
    ui->LEA2M_label->setText(LEA_2M_Label);

    QString PowerErg2M_Cond_1=QString::number(MyLaserMP_Pr->getPowerErg_Cond_1()[1], 'e', 2);
    QString PowerErg2M_Cond_3=QString::number(MyLaserMP_Pr->getPowerErg_Cond_3()[1], 'e', 2);

    QString PowerErg2M_Cond_1_Label=QString("%1=%2 %3")
                             .arg(Formula2M_Tipo)
                             .arg(PowerErg2M_Cond_1)
                             .arg(Formula2M_Unit);

    QString PowerErg2M_Cond_3_Label=QString("%1=%2 %3")
                             .arg(Formula2M_Tipo)
                             .arg(PowerErg2M_Cond_3)
                             .arg(Formula2M_Unit);

    ui->cond1LEA_2M_Label->setText(PowerErg2M_Cond_1_Label);
    ui->cond3LEA_2M_Label->setText(PowerErg2M_Cond_3_Label);

    QString tPowerErg2M_Cond_1_Label=QString("%1<sub>Acc 1</sub>").arg(Formula2M_Tipo);
    ui->tCond1LEA_2M_Label->setText(tPowerErg2M_Cond_1_Label);

    QString tPowerErg2M_Cond_3_Label=QString("%1<sub>Acc 3</sub>").arg(Formula2M_Tipo);
    ui->tCond3LEA_2M_Label->setText(tPowerErg2M_Cond_3_Label);

    QString Formula3R_Tipo=QString::fromStdString(MyLaserMP_Pr->getLEA_FormulaTipo()[2]);
    QString Formula3R=QString::fromStdString(MyLaserMP_Pr->getLEA_Formula()[2]);
    QString Formula3R_Unit=QString::fromStdString(MyLaserMP_Pr->getLEA_FormulaUnit()[2]);
    QString LEA_3R=QString::number(MyLaserMP_Pr->getLEA()[2], 'e', 2);
    QString Formula3R_Label=QString("%1=%2 %3")
                           .arg(Formula3R_Tipo)
                           .arg(Formula3R)
                           .arg(Formula3R_Unit);

    QString LEA_3R_Label=QString("%1=%2 %3")
                            .arg(Formula3R_Tipo)
                            .arg(LEA_3R)
                            .arg(Formula3R_Unit);

    ui->Formula3R_Label->setText(Formula3R_Label);
    ui->LEA3R_label->setText(LEA_3R_Label);

    QString PowerErg3R_Cond_1=QString::number(MyLaserMP_Pr->getPowerErg_Cond_1()[2], 'e', 2);
    QString PowerErg3R_Cond_3=QString::number(MyLaserMP_Pr->getPowerErg_Cond_3()[2], 'e', 2);

    QString PowerErg3R_Cond_1_Label=QString("%1=%2 %3")
                         .arg(Formula3R_Tipo)
                         .arg(PowerErg3R_Cond_1)
                         .arg(Formula3R_Unit);

    QString PowerErg3R_Cond_3_Label=QString("%1=%2 %3")
                         .arg(Formula3R_Tipo)
                         .arg(PowerErg3R_Cond_3)
                         .arg(Formula3R_Unit);

    ui->cond1LEA_3R_Label->setText(PowerErg3R_Cond_1_Label);
    ui->cond3LEA_3R_Label->setText(PowerErg3R_Cond_3_Label);

    QString tPowerErg3R_Cond_1_Label=QString("%1<sub>Acc 1</sub>").arg(Formula3R_Tipo);
    ui->tCond1LEA_3R_Label->setText(tPowerErg3R_Cond_1_Label);

    QString tPowerErg3R_Cond_3_Label=QString("%1<sub>Acc 3</sub>").arg(Formula3R_Tipo);
    ui->tCond3LEA_3R_Label->setText(tPowerErg3R_Cond_3_Label);

    QString Formula3B_Tipo=QString::fromStdString(MyLaserMP_Pr->getLEA_FormulaTipo()[3]);
    QString Formula3B=QString::fromStdString(MyLaserMP_Pr->getLEA_Formula()[3]);
    QString Formula3B_Unit=QString::fromStdString(MyLaserMP_Pr->getLEA_FormulaUnit()[3]);
    QString LEA_3B=QString::number(MyLaserMP_Pr->getLEA()[3], 'e', 2);
    QString Formula3B_Label=QString("%1=%2 %3")
                            .arg(Formula3B_Tipo)
                            .arg(Formula3B)
                            .arg(Formula3B_Unit);

    QString LEA_3B_Label=QString("%1=%2 %3")
                            .arg(Formula3B_Tipo)
                            .arg(LEA_3B)
                            .arg(Formula3R_Unit);

    ui->Formula3B_Label->setText(Formula3B_Label);
    ui->LEA3B_label->setText(LEA_3B_Label);

    QString PowerErg3B_Cond_1=QString::number(MyLaserMP_Pr->getPowerErg_Cond_1()[3], 'e', 2);
    QString PowerErg3B_Cond_3=QString::number(MyLaserMP_Pr->getPowerErg_Cond_3()[3], 'e', 2);

    QString PowerErg3B_Cond_1_Label=QString("%1=%2 %3")
                         .arg(Formula3B_Tipo)
                         .arg(PowerErg3B_Cond_1)
                         .arg(Formula3B_Unit);

    QString PowerErg3B_Cond_3_Label=QString("%1=%2 %3")
                         .arg(Formula3B_Tipo)
                         .arg(PowerErg3B_Cond_3)
                         .arg(Formula3B_Unit);

    ui->cond1LEA_3B_Label->setText(PowerErg3B_Cond_1_Label);
    ui->cond3LEA_3B_Label->setText(PowerErg3B_Cond_3_Label);

    QString tPowerErg3B_Cond_1_Label=QString("%1<sub>Acc 1</sub>").arg(Formula3B_Tipo);
    ui->tCond1LEA_3B_Label->setText(tPowerErg3B_Cond_1_Label);

    QString tPowerErg3B_Cond_3_Label=QString("%1<sub>Acc 3</sub>").arg(Formula3B_Tipo);
    ui->tCond3LEA_3B_Label->setText(tPowerErg3B_Cond_3_Label);

    ui->SkinDamage_Label->setText(QString::fromStdString(MyLaserMP_Pr->getSkinDamage()));
    ui->EyeDamage_Label->setText(QString::fromStdString(MyLaserMP_Pr->getEyeDamage()));
    ui->Radiation_Label->setText(QString::fromStdString(MyLaserMP_Pr->getRadiation()));
    ui->class_Label->setText(QString::fromStdString(MyLaserMP_Pr->getLaserClass()));


     /*****************************************************
      * valutazione relativa l'emissione media del laser  *
      * ***************************************************/

    ui->couplingFactor1_Label_2->setText(QString::number(MyLaserMP_Pr->getMeanCouplingFactor_Cond_1(), 'e', 2));
    ui->couplingFactor3_Label_2->setText(QString::number(MyLaserMP_Pr->getMeanCouplingFactor_Cond_3(), 'e', 2));
    ui->apertureDiam1_Label_2->setText(QString::number(MyLaserMP_Pr->getMeanApCond_1(), 'e', 2)+" mm");
    ui->apertureDiam3_Label_2->setText(QString::number(MyLaserMP_Pr->getMeanApCond_3(), 'e', 2)+" mm");
    ui->apertureDist1_Label_2->setText(QString::number(MyLaserMP_Pr->getMeanDistCond_1(), 'e', 2)+" mm");
    ui->apertureDist3_Label_2->setText(QString::number(MyLaserMP_Pr->getMeanDistCond_3(), 'e', 2)+" mm");
    ui->beamAperture1_Label_2->setText(QString::number(MyLaserMP_Pr->getMeanBeamAtStop_Cond_1(), 'e', 2)+" mm");
    ui->beamAperture3_Label_2->setText(QString::number(MyLaserMP_Pr->getMeanBeamAtStop_Cond_3(), 'e', 2)+" mm");

    QString Formula1M_Tipo_2=QString::fromStdString(MyLaserMP_Pr->getMeanLEA_FormulaTipo()[0]);
    QString Formula1M_2=QString::fromStdString(MyLaserMP_Pr->getMeanLEA_Formula()[0]);
    QString Formula1M_Unit_2=QString::fromStdString(MyLaserMP_Pr->getMeanLEA_FormulaUnit()[0]);
    QString LEA_1M_2=QString::number(MyLaserMP_Pr->getMeanLEA()[0], 'e', 2);
    QString Formula1M_Label_2=QString("%1=%2 %3")
                         .arg(Formula1M_Tipo_2)
                         .arg(Formula1M_2)
                         .arg(Formula1M_Unit_2);
         QString LEA_1M_Label_2=QString("%1=%2 %3")
                         .arg(Formula1M_Tipo_2)
                         .arg(LEA_1M_2)
                         .arg(Formula1M_Unit_2);
    ui->Formula1M_Label_2->setText(Formula1M_Label_2);
    ui->LEA1M_label_2->setText(LEA_1M_Label_2);
    QString PowerErg1M_Cond_1_2=QString::number(MyLaserMP_Pr->getMeanPowerErg_Cond_1()[0], 'e', 2);
    QString PowerErg1M_Cond_3_2=QString::number(MyLaserMP_Pr->getMeanPowerErg_Cond_3()[0], 'e', 2);
    QString PowerErg1M_Cond_1_Label_2=QString("%1=%2 %3")
                         .arg(Formula1M_Tipo_2)
                         .arg(PowerErg1M_Cond_1_2)
                         .arg(Formula1M_Unit_2);
    QString PowerErg1M_Cond_3_Label_2=QString("%1=%2 %3")
                         .arg(Formula1M_Tipo_2)
                         .arg(PowerErg1M_Cond_3_2)
                         .arg(Formula1M_Unit_2);
    ui->cond1LEA_1M_Label_2->setText(PowerErg1M_Cond_1_Label_2);
    ui->cond3LEA_1M_Label_2->setText(PowerErg1M_Cond_3_Label_2);

    QString tPowerErg1M_Cond_1_Label_2=QString("%1<sub>Acc 1</sub>").arg(Formula1M_Tipo_2);
    ui->tCond1LEA_1M_Label_2->setText(tPowerErg1M_Cond_1_Label_2);

    QString tPowerErg1M_Cond_3_Label_2=QString("%1<sub>Acc 3</sub>").arg(Formula1M_Tipo_2);
    ui->tCond3LEA_1M_Label_2->setText(tPowerErg1M_Cond_3_Label_2);

    QString Formula2M_Tipo_2=QString::fromStdString(MyLaserMP_Pr->getMeanLEA_FormulaTipo()[1]);
    QString Formula2M_2=QString::fromStdString(MyLaserMP_Pr->getMeanLEA_Formula()[1]);
    QString Formula2M_Unit_2=QString::fromStdString(MyLaserMP_Pr->getMeanLEA_FormulaUnit()[1]);
    QString LEA_2M_2=QString::number(MyLaserMP_Pr->getMeanLEA()[1], 'e', 2);

    QString Formula2M_Label_2;
    QString LEA_2M_Label_2;

    Formula2M_Label_2=QString("%1=%2 %3")
                           .arg(Formula2M_Tipo_2)
                           .arg(Formula2M_2)
                           .arg(Formula2M_Unit_2);

    LEA_2M_Label_2=QString("%1=%2 %3")
                           .arg(Formula2M_Tipo_2)
                           .arg(LEA_2M_2)
                           .arg(Formula2M_Unit_2);

    ui->Formula2M_Label_2->setText(Formula2M_Label_2);
    ui->LEA2M_label_2->setText(LEA_2M_Label_2);

    QString PowerErg2M_Cond_1_2=QString::number(MyLaserMP_Pr->getMeanPowerErg_Cond_1()[1], 'e', 2);
    QString PowerErg2M_Cond_3_2=QString::number(MyLaserMP_Pr->getMeanPowerErg_Cond_3()[1], 'e', 2);

    QString PowerErg2M_Cond_1_Label_2=QString("%1=%2 %3")
                             .arg(Formula2M_Tipo_2)
                             .arg(PowerErg2M_Cond_1_2)
                             .arg(Formula2M_Unit_2);

    QString PowerErg2M_Cond_3_Label_2=QString("%1=%2 %3")
                             .arg(Formula2M_Tipo_2)
                             .arg(PowerErg2M_Cond_3_2)
                             .arg(Formula2M_Unit_2);

    ui->cond1LEA_2M_Label_2->setText(PowerErg2M_Cond_1_Label_2);
    ui->cond3LEA_2M_Label_2->setText(PowerErg2M_Cond_3_Label_2);

    QString tPowerErg2M_Cond_1_Label_2=QString("%1<sub>Acc 1</sub>").arg(Formula2M_Tipo_2);
    ui->tCond1LEA_2M_Label_2->setText(tPowerErg2M_Cond_1_Label_2);

    QString tPowerErg2M_Cond_3_Label_2=QString("%1<sub>Acc 3</sub>").arg(Formula2M_Tipo_2);
    ui->tCond3LEA_2M_Label_2->setText(tPowerErg2M_Cond_3_Label_2);

    QString Formula3R_Tipo_2=QString::fromStdString(MyLaserMP_Pr->getMeanLEA_FormulaTipo()[2]);
    QString Formula3R_2=QString::fromStdString(MyLaserMP_Pr->getMeanLEA_Formula()[2]);
    QString Formula3R_Unit_2=QString::fromStdString(MyLaserMP_Pr->getMeanLEA_FormulaUnit()[2]);
    QString LEA_3R_2=QString::number(MyLaserMP_Pr->getMeanLEA()[2], 'e', 2);
    QString Formula3R_Label_2=QString("%1=%2 %3")
                           .arg(Formula3R_Tipo_2)
                           .arg(Formula3R_2)
                           .arg(Formula3R_Unit_2);

    QString LEA_3R_Label_2=QString("%1=%2 %3")
                            .arg(Formula3R_Tipo_2)
                            .arg(LEA_3R_2)
                            .arg(Formula3R_Unit_2);

    ui->Formula3R_Label_2->setText(Formula3R_Label_2);
    ui->LEA3R_label_2->setText(LEA_3R_Label_2);

    QString PowerErg3R_Cond_1_2=QString::number(MyLaserMP_Pr->getMeanPowerErg_Cond_1()[2], 'e', 2);
    QString PowerErg3R_Cond_3_2=QString::number(MyLaserMP_Pr->getMeanPowerErg_Cond_3()[2], 'e', 2);

    QString PowerErg3R_Cond_1_Label_2=QString("%1=%2 %3")
                         .arg(Formula3R_Tipo_2)
                         .arg(PowerErg3R_Cond_1_2)
                         .arg(Formula3R_Unit_2);

    QString PowerErg3R_Cond_3_Label_2=QString("%1=%2 %3")
                         .arg(Formula3R_Tipo_2)
                         .arg(PowerErg3R_Cond_3_2)
                         .arg(Formula3R_Unit_2);

    ui->cond1LEA_3R_Label_2->setText(PowerErg3R_Cond_1_Label_2);
    ui->cond3LEA_3R_Label_2->setText(PowerErg3R_Cond_3_Label_2);

    QString tPowerErg3R_Cond_1_Label_2=QString("%1<sub>Acc 1</sub>").arg(Formula3R_Tipo_2);
    ui->tCond1LEA_3R_Label_2->setText(tPowerErg3R_Cond_1_Label_2);

    QString tPowerErg3R_Cond_3_Label_2=QString("%1<sub>Acc 3</sub>").arg(Formula3R_Tipo_2);
    ui->tCond3LEA_3R_Label_2->setText(tPowerErg3R_Cond_3_Label_2);

    QString Formula3B_Tipo_2=QString::fromStdString(MyLaserMP_Pr->getMeanLEA_FormulaTipo()[3]);
    QString Formula3B_2=QString::fromStdString(MyLaserMP_Pr->getMeanLEA_Formula()[3]);
    QString Formula3B_Unit_2=QString::fromStdString(MyLaserMP_Pr->getMeanLEA_FormulaUnit()[3]);
    QString LEA_3B_2=QString::number(MyLaserMP_Pr->getMeanLEA()[3], 'e', 2);
    QString Formula3B_Label_2=QString("%1=%2 %3")
                            .arg(Formula3B_Tipo_2)
                            .arg(Formula3B_2)
                            .arg(Formula3B_Unit_2);

    QString LEA_3B_Label_2=QString("%1=%2 %3")
                            .arg(Formula3B_Tipo_2)
                            .arg(LEA_3B_2)
                            .arg(Formula3R_Unit_2);

    ui->Formula3B_Label_2->setText(Formula3B_Label_2);
    ui->LEA3B_label_2->setText(LEA_3B_Label_2);

    QString PowerErg3B_Cond_1_2=QString::number(MyLaserMP_Pr->getMeanPowerErg_Cond_1()[3], 'e', 2);
    QString PowerErg3B_Cond_3_2=QString::number(MyLaserMP_Pr->getMeanPowerErg_Cond_3()[3], 'e', 2);

    QString PowerErg3B_Cond_1_Label_2=QString("%1=%2 %3")
                         .arg(Formula3B_Tipo_2)
                         .arg(PowerErg3B_Cond_1_2)
                         .arg(Formula3B_Unit_2);

    QString PowerErg3B_Cond_3_Label_2=QString("%1=%2 %3")
                         .arg(Formula3B_Tipo_2)
                         .arg(PowerErg3B_Cond_3_2)
                         .arg(Formula3B_Unit_2);

    ui->cond1LEA_3B_Label_2->setText(PowerErg3B_Cond_1_Label_2);
    ui->cond3LEA_3B_Label_2->setText(PowerErg3B_Cond_3_Label_2);

    QString tPowerErg3B_Cond_1_Label_2=QString("%1<sub>Acc 1</sub>").arg(Formula3B_Tipo_2);
    ui->tCond1LEA_3B_Label_2->setText(tPowerErg3B_Cond_1_Label_2);

    QString tPowerErg3B_Cond_3_Label_2=QString("%1<sub>Acc 3</sub>").arg(Formula3B_Tipo_2);
    ui->tCond3LEA_3B_Label_2->setText(tPowerErg3B_Cond_3_Label_2);

    ui->class_Label_2->setText(QString::fromStdString(MyLaserMP_Pr->getMeanLaserClassAssigned()));


    ui->MeanPowerLabel->setText(QString::number(MyLaserMP_Pr->getMeanPower(),'e', 2)+" W");
    ui->CountingLabel->setText(QString::fromStdString(MyLaserMP_Pr->valutateCounting()));

            if((wavelength>=400)and(wavelength<=1400)){
                ui->C5_Label->setText(QString::number(MyLaserMP_Pr->getC5Coefficient(),'e', 2));}
                else{
                ui->C5_Label->setText("Non applicabile");}

    ui->PulseNumberLabel->setText(QString::number(MyLaserMP_Pr->getPulseNumber()));
}

void DockLea::setWidgetsForThermal()
{

    /********************************************************************
     * valutazione relativa agli effetti termici del laser per prf<1/Ti *
     * ******************************************************************/

    ui->couplingFactor1_Label_3->setText(QString::number(MyLaserMP_Pr->getCouplingFactor_Cond_1(), 'e', 2));
    ui->couplingFactor3_Label_3->setText(QString::number(MyLaserMP_Pr->getCouplingFactor_Cond_3(), 'e', 2));
    ui->apertureDiam1_Label_3->setText(QString::number(MyLaserMP_Pr->getApCond_1(), 'e', 2)+" mm");
    ui->apertureDiam3_Label_3->setText(QString::number(MyLaserMP_Pr->getApCond_3(), 'e', 2)+" mm");
    ui->apertureDist1_Label_3->setText(QString::number(MyLaserMP_Pr->getDistCond_1(), 'e', 2)+" mm");
    ui->apertureDist3_Label_3->setText(QString::number(MyLaserMP_Pr->getDistCond_3(), 'e', 2)+" mm");
    ui->beamAperture1_Label_3->setText(QString::number(MyLaserMP_Pr->getBeamAtStop_Cond_1(), 'e', 2)+" mm");
    ui->beamAperture3_Label_3->setText(QString::number(MyLaserMP_Pr->getBeamAtStop_Cond_3(), 'e', 2)+" mm");

    QString Formula1M_Tipo_3=QString::fromStdString(MyLaserMP_Pr->getLEA_FormulaTipo()[0]);
    QString Formula1M_3=QString::fromStdString(MyLaserMP_Pr->getLEA_Formula()[0]);
    QString Formula1M_Unit_3=QString::fromStdString(MyLaserMP_Pr->getLEA_FormulaUnit()[0]);
    QString LEA_1M_3=QString::number(MyLaserMP_Pr->getLEA_Corrected()[0], 'e', 2);
    QString Formula1M_Label_3=QString("%1=%2 C<sub>5</sub> %3")
                        .arg(Formula1M_Tipo_3)
                        .arg(Formula1M_3)
                        .arg(Formula1M_Unit_3);
        QString LEA_1M_Label_3=QString("%1=%2 %3")
                        .arg(Formula1M_Tipo_3)
                        .arg(LEA_1M_3)
                        .arg(Formula1M_Unit_3);
    ui->Formula1M_Label_3->setText(Formula1M_Label_3);
    ui->LEA1M_label_3->setText(LEA_1M_Label_3);
    QString PowerErg1M_Cond_1_3=QString::number(MyLaserMP_Pr->getPowerErg_Cond_1()[0], 'e', 2);
    QString PowerErg1M_Cond_3_3=QString::number(MyLaserMP_Pr->getPowerErg_Cond_3()[0], 'e', 2);
    QString PowerErg1M_Cond_1_Label_3=QString("%1=%2 C<sub>5</sub> %3")
                        .arg(Formula1M_Tipo_3)
                        .arg(PowerErg1M_Cond_1_3)
                        .arg(Formula1M_Unit_3);
    QString PowerErg1M_Cond_3_Label_3=QString("%1=%2 %3")
                        .arg(Formula1M_Tipo_3)
                        .arg(PowerErg1M_Cond_3_3)
                        .arg(Formula1M_Unit_3);
    ui->cond1LEA_1M_Label_3->setText(PowerErg1M_Cond_1_Label_3);
    ui->cond3LEA_1M_Label_3->setText(PowerErg1M_Cond_3_Label_3);

    QString tPowerErg1M_Cond_1_Label_3=QString("%1<sub>Acc 1</sub>").arg(Formula1M_Tipo_3);
    ui->tCond1LEA_1M_Label_3->setText(tPowerErg1M_Cond_1_Label_3);

    QString tPowerErg1M_Cond_3_Label_3=QString("%1<sub>Acc 3</sub>").arg(Formula1M_Tipo_3);
    ui->tCond3LEA_1M_Label_3->setText(tPowerErg1M_Cond_3_Label_3);

    QString Formula2M_Tipo_3=QString::fromStdString(MyLaserMP_Pr->getLEA_FormulaTipo()[1]);
    QString Formula2M_3=QString::fromStdString(MyLaserMP_Pr->getLEA_Formula()[1]);
    QString Formula2M_Unit_3=QString::fromStdString(MyLaserMP_Pr->getLEA_FormulaUnit()[1]);
    QString LEA_2M_3=QString::number(MyLaserMP_Pr->getLEA_Corrected()[1], 'e', 2);

    QString Formula2M_Label_3;
    QString LEA_2M_Label_3;

    Formula2M_Label_3=QString("%1=%2 %3")
                          .arg(Formula2M_Tipo_3)
                          .arg(Formula2M_3)
                          .arg(Formula2M_Unit_3);

   LEA_2M_Label_3=QString("%1=%2 %3")
                          .arg(Formula2M_Tipo_3)
                          .arg(LEA_2M_3)
                          .arg(Formula2M_Unit_3);

    ui->Formula2M_Label_3->setText(Formula2M_Label_3);
    ui->LEA2M_label_3->setText(LEA_2M_Label_3);

    QString PowerErg2M_Cond_1_3=QString::number(MyLaserMP_Pr->getPowerErg_Cond_1()[1], 'e', 2);
    QString PowerErg2M_Cond_3_3=QString::number(MyLaserMP_Pr->getPowerErg_Cond_3()[1], 'e', 2);

    QString PowerErg2M_Cond_1_Label_3=QString("%1=%2 C<sub>5</sub> %3")
                            .arg(Formula2M_Tipo_3)
                            .arg(PowerErg2M_Cond_1_3)
                            .arg(Formula2M_Unit_3);

    QString PowerErg2M_Cond_3_Label_3=QString("%1=%2 %3")
                            .arg(Formula2M_Tipo_3)
                            .arg(PowerErg2M_Cond_3_3)
                            .arg(Formula2M_Unit_3);

    ui->cond1LEA_2M_Label_3->setText(PowerErg2M_Cond_1_Label_3);
    ui->cond3LEA_2M_Label_3->setText(PowerErg2M_Cond_3_Label_3);

    QString tPowerErg2M_Cond_1_Label_3=QString("%1<sub>Acc 1</sub>").arg(Formula2M_Tipo_3);
    ui->tCond1LEA_2M_Label_3->setText(tPowerErg2M_Cond_1_Label_3);

    QString tPowerErg2M_Cond_3_Label_3=QString("%1<sub>Acc 3</sub>").arg(Formula2M_Tipo_3);
    ui->tCond3LEA_2M_Label_3->setText(tPowerErg2M_Cond_3_Label_3);

    QString Formula3R_Tipo_3=QString::fromStdString(MyLaserMP_Pr->getLEA_FormulaTipo()[2]);
    QString Formula3R_3=QString::fromStdString(MyLaserMP_Pr->getLEA_Formula()[2]);
    QString Formula3R_Unit_3=QString::fromStdString(MyLaserMP_Pr->getLEA_FormulaUnit()[2]);
    QString LEA_3R_3=QString::number(MyLaserMP_Pr->getLEA_Corrected()[2], 'e', 2);
    QString Formula3R_Label_3=QString("%1=%2 C<sub>5</sub> %3")
                          .arg(Formula3R_Tipo_3)
                          .arg(Formula3R_3)
                          .arg(Formula3R_Unit_3);

    QString LEA_3R_Label_3=QString("%1=%2 %3")
                           .arg(Formula3R_Tipo_3)
                           .arg(LEA_3R_3)
                           .arg(Formula3R_Unit_3);

    ui->Formula3R_Label_3->setText(Formula3R_Label_3);
    ui->LEA3R_label_3->setText(LEA_3R_Label_3);

    QString PowerErg3R_Cond_1_3=QString::number(MyLaserMP_Pr->getPowerErg_Cond_1()[2], 'e', 2);
    QString PowerErg3R_Cond_3_3=QString::number(MyLaserMP_Pr->getPowerErg_Cond_3()[2], 'e', 2);

    QString PowerErg3R_Cond_1_Label_3=QString("%1=%2 C<sub>5</sub> %3")
                        .arg(Formula3R_Tipo_3)
                        .arg(PowerErg3R_Cond_1_3)
                        .arg(Formula3R_Unit_3);

    QString PowerErg3R_Cond_3_Label_3=QString("%1=%2 %3")
                        .arg(Formula3R_Tipo_3)
                        .arg(PowerErg3R_Cond_3_3)
                        .arg(Formula3R_Unit_3);

    ui->cond1LEA_3R_Label_3->setText(PowerErg3R_Cond_1_Label_3);
    ui->cond3LEA_3R_Label_3->setText(PowerErg3R_Cond_3_Label_3);

    QString tPowerErg3R_Cond_1_Label_3=QString("%1<sub>Acc 1</sub>").arg(Formula3R_Tipo_3);
    ui->tCond1LEA_3R_Label_3->setText(tPowerErg3R_Cond_1_Label_3);

    QString tPowerErg3R_Cond_3_Label_3=QString("%1<sub>Acc 3</sub>").arg(Formula3R_Tipo_3);
    ui->tCond3LEA_3R_Label_3->setText(tPowerErg3R_Cond_3_Label_3);

    QString Formula3B_Tipo_3=QString::fromStdString(MyLaserMP_Pr->getLEA_FormulaTipo()[3]);
    QString Formula3B_3=QString::fromStdString(MyLaserMP_Pr->getLEA_Formula()[3]);
    QString Formula3B_Unit_3=QString::fromStdString(MyLaserMP_Pr->getLEA_FormulaUnit()[3]);
    QString LEA_3B_3=QString::number(MyLaserMP_Pr->getLEA_Corrected()[3], 'e', 2);
    QString Formula3B_Label_3=QString("%1=%2 C<sub>5</sub> %3")
                           .arg(Formula3B_Tipo_3)
                           .arg(Formula3B_3)
                           .arg(Formula3B_Unit_3);

    QString LEA_3B_Label_3=QString("%1=%2 %3")
                           .arg(Formula3B_Tipo_3)
                           .arg(LEA_3B_3)
                           .arg(Formula3R_Unit_3);

    ui->Formula3B_Label_3->setText(Formula3B_Label_3);
    ui->LEA3B_label_3->setText(LEA_3B_Label_3);

    QString PowerErg3B_Cond_1_3=QString::number(MyLaserMP_Pr->getPowerErg_Cond_1()[3], 'e', 2);
    QString PowerErg3B_Cond_3_3=QString::number(MyLaserMP_Pr->getPowerErg_Cond_3()[3], 'e', 2);

    QString PowerErg3B_Cond_1_Label_3=QString("%1=%2 C<sub>5</sub> %3")
                        .arg(Formula3B_Tipo_3)
                        .arg(PowerErg3B_Cond_1_3)
                        .arg(Formula3B_Unit_3);

    QString PowerErg3B_Cond_3_Label_3=QString("%1=%2 %3")
                        .arg(Formula3B_Tipo_3)
                        .arg(PowerErg3B_Cond_3_3)
                        .arg(Formula3B_Unit_3);

    ui->cond1LEA_3B_Label_3->setText(PowerErg3B_Cond_1_Label_3);
    ui->cond3LEA_3B_Label_3->setText(PowerErg3B_Cond_3_Label_3);

    QString tPowerErg3B_Cond_1_Label_3=QString("%1<sub>Acc 1</sub>").arg(Formula3B_Tipo_3);
    ui->tCond1LEA_3B_Label_3->setText(tPowerErg3B_Cond_1_Label_3);

    QString tPowerErg3B_Cond_3_Label_3=QString("%1<sub>Acc 3</sub>").arg(Formula3B_Tipo_3);
    ui->tCond3LEA_3B_Label_3->setText(tPowerErg3B_Cond_3_Label_3);


    ui->class_Label_3->setText(QString::fromStdString(MyLaserMP_Pr->getC5LaserClassAssigned()));
}



void DockLea::setWidgetsForThermalTi()
{
        /********************************************************************
         * valutazione relativa agli effetti termici del laser per prf>1/Ti *
         * ******************************************************************/

       ui->couplingFactor1_Label_3->setText(QString::number(MyLaserMP_Pr->getTiCouplingFactor_Cond_1(), 'e', 2));
       ui->couplingFactor3_Label_3->setText(QString::number(MyLaserMP_Pr->getTiCouplingFactor_Cond_3(), 'e', 2));
       ui->apertureDiam1_Label_3->setText(QString::number(MyLaserMP_Pr->getTiApCond_1(), 'e', 2)+" m");
       ui->apertureDiam3_Label_3->setText(QString::number(MyLaserMP_Pr->getTiApCond_3(), 'e', 2)+" m");
       ui->apertureDist1_Label_3->setText(QString::number(MyLaserMP_Pr->getTiDistCond_1(), 'e', 2)+" m");
       ui->apertureDist3_Label_3->setText(QString::number(MyLaserMP_Pr->getTiDistCond_3(), 'e', 2)+" m");
       ui->beamAperture1_Label_3->setText(QString::number(MyLaserMP_Pr->getTiBeamAtStop_Cond_1(), 'e', 2)+" m");
       ui->beamAperture3_Label_3->setText(QString::number(MyLaserMP_Pr->getTiBeamAtStop_Cond_3(), 'e', 2)+" m");

       QString Formula1M_Tipo_3=QString::fromStdString(MyLaserMP_Pr->getTiLEA_FormulaTipo()[0]);
       QString Formula1M_3=QString::fromStdString(MyLaserMP_Pr->getTiLEA_Formula()[0]);
       QString Formula1M_Unit_3=QString::fromStdString(MyLaserMP_Pr->getTiLEA_FormulaUnit()[0]);
       QString LEA_1M_3=QString::number(MyLaserMP_Pr->getTiLEA_Corrected()[0], 'e', 2);
       QString Formula1M_Label_3=QString("%1=%2 C<sub>5</sub> %3")
                            .arg(Formula1M_Tipo_3)
                            .arg(Formula1M_3)
                            .arg(Formula1M_Unit_3);
            QString LEA_1M_Label_3=QString("%1=%2 %3")
                            .arg(Formula1M_Tipo_3)
                            .arg(LEA_1M_3)
                            .arg(Formula1M_Unit_3);
       ui->Formula1M_Label_3->setText(Formula1M_Label_3);
       ui->LEA1M_label_3->setText(LEA_1M_Label_3);
       QString PowerErg1M_Cond_1_3=QString::number(MyLaserMP_Pr->getTiPowerErg_Cond_1()[0], 'e', 2);
       QString PowerErg1M_Cond_3_3=QString::number(MyLaserMP_Pr->getTiPowerErg_Cond_3()[0], 'e', 2);
       QString PowerErg1M_Cond_1_Label_3=QString("%1=%2 C<sub>5</sub> %3")
                            .arg(Formula1M_Tipo_3)
                            .arg(PowerErg1M_Cond_1_3)
                            .arg(Formula1M_Unit_3);
       QString PowerErg1M_Cond_3_Label_3=QString("%1=%2 %3")
                            .arg(Formula1M_Tipo_3)
                            .arg(PowerErg1M_Cond_3_3)
                            .arg(Formula1M_Unit_3);
       ui->cond1LEA_1M_Label_3->setText(PowerErg1M_Cond_1_Label_3);
       ui->cond3LEA_1M_Label_3->setText(PowerErg1M_Cond_3_Label_3);

       QString tPowerErg1M_Cond_1_Label_3=QString("%1<sub>Acc 1</sub>").arg(Formula1M_Tipo_3);
       ui->tCond1LEA_1M_Label_3->setText(tPowerErg1M_Cond_1_Label_3);

       QString tPowerErg1M_Cond_3_Label_3=QString("%1<sub>Acc 3</sub>").arg(Formula1M_Tipo_3);
       ui->tCond3LEA_1M_Label_3->setText(tPowerErg1M_Cond_3_Label_3);

       QString Formula2M_Tipo_3=QString::fromStdString(MyLaserMP_Pr->getTiLEA_FormulaTipo()[1]);
       QString Formula2M_3=QString::fromStdString(MyLaserMP_Pr->getTiLEA_Formula()[1]);
       QString Formula2M_Unit_3=QString::fromStdString(MyLaserMP_Pr->getTiLEA_FormulaUnit()[1]);
       QString LEA_2M_3=QString::number(MyLaserMP_Pr->getTiLEA_Corrected()[1], 'e', 2);

       QString Formula2M_Label_3;
       QString LEA_2M_Label_3;

       Formula2M_Label_3=QString("%1=%2 C<sub>5</sub> %3")
                              .arg(Formula2M_Tipo_3)
                              .arg(Formula2M_3)
                              .arg(Formula2M_Unit_3);

       LEA_2M_Label_3=QString("%1=%2 %3")
                              .arg(Formula2M_Tipo_3)
                              .arg(LEA_2M_3)
                              .arg(Formula2M_Unit_3);

       ui->Formula2M_Label_3->setText(Formula2M_Label_3);
       ui->LEA2M_label_3->setText(LEA_2M_Label_3);

       QString PowerErg2M_Cond_1_3=QString::number(MyLaserMP_Pr->getTiPowerErg_Cond_1()[1], 'e', 2);
       QString PowerErg2M_Cond_3_3=QString::number(MyLaserMP_Pr->getTiPowerErg_Cond_3()[1], 'e', 2);

       QString PowerErg2M_Cond_1_Label_3=QString("%1=%2 C<sub>5</sub> %3")
                                .arg(Formula2M_Tipo_3)
                                .arg(PowerErg2M_Cond_1_3)
                                .arg(Formula2M_Unit_3);

       QString PowerErg2M_Cond_3_Label_3=QString("%1=%2 %3")
                                .arg(Formula2M_Tipo_3)
                                .arg(PowerErg2M_Cond_3_3)
                                .arg(Formula2M_Unit_3);

       ui->cond1LEA_2M_Label_3->setText(PowerErg2M_Cond_1_Label_3);
       ui->cond3LEA_2M_Label_3->setText(PowerErg2M_Cond_3_Label_3);

       QString tPowerErg2M_Cond_1_Label_3=QString("%1<sub>Acc 1</sub>").arg(Formula2M_Tipo_3);
       ui->tCond1LEA_2M_Label_3->setText(tPowerErg2M_Cond_1_Label_3);

       QString tPowerErg2M_Cond_3_Label_3=QString("%1<sub>Acc 3</sub>").arg(Formula2M_Tipo_3);
       ui->tCond3LEA_2M_Label_3->setText(tPowerErg2M_Cond_3_Label_3);

       QString Formula3R_Tipo_3=QString::fromStdString(MyLaserMP_Pr->getTiLEA_FormulaTipo()[2]);
       QString Formula3R_3=QString::fromStdString(MyLaserMP_Pr->getTiLEA_Formula()[2]);
       QString Formula3R_Unit_3=QString::fromStdString(MyLaserMP_Pr->getTiLEA_FormulaUnit()[2]);
       QString LEA_3R_3=QString::number(MyLaserMP_Pr->getTiLEA_Corrected()[2], 'e', 2);
       QString Formula3R_Label_3=QString("%1=%2 C<sub>5</sub> %3")
                              .arg(Formula3R_Tipo_3)
                              .arg(Formula3R_3)
                              .arg(Formula3R_Unit_3);

       QString LEA_3R_Label_3=QString("%1=%2 %3")
                               .arg(Formula3R_Tipo_3)
                               .arg(LEA_3R_3)
                               .arg(Formula3R_Unit_3);

       ui->Formula3R_Label_3->setText(Formula3R_Label_3);
       ui->LEA3R_label_3->setText(LEA_3R_Label_3);

       QString PowerErg3R_Cond_1_3=QString::number(MyLaserMP_Pr->getTiPowerErg_Cond_1()[2], 'e', 2);
       QString PowerErg3R_Cond_3_3=QString::number(MyLaserMP_Pr->getTiPowerErg_Cond_3()[2], 'e', 2);

       QString PowerErg3R_Cond_1_Label_3=QString("%1=%2 C<sub>5</sub> %3")
                            .arg(Formula3R_Tipo_3)
                            .arg(PowerErg3R_Cond_1_3)
                            .arg(Formula3R_Unit_3);

       QString PowerErg3R_Cond_3_Label_3=QString("%1=%2 %3")
                            .arg(Formula3R_Tipo_3)
                            .arg(PowerErg3R_Cond_3_3)
                            .arg(Formula3R_Unit_3);

       ui->cond1LEA_3R_Label_3->setText(PowerErg3R_Cond_1_Label_3);
       ui->cond3LEA_3R_Label_3->setText(PowerErg3R_Cond_3_Label_3);

       QString tPowerErg3R_Cond_1_Label_3=QString("%1<sub>Acc 1</sub>").arg(Formula3R_Tipo_3);
       ui->tCond1LEA_3R_Label_3->setText(tPowerErg3R_Cond_1_Label_3);

       QString tPowerErg3R_Cond_3_Label_3=QString("%1<sub>Acc 3</sub>").arg(Formula3R_Tipo_3);
       ui->tCond3LEA_3R_Label_3->setText(tPowerErg3R_Cond_3_Label_3);

       QString Formula3B_Tipo_3=QString::fromStdString(MyLaserMP_Pr->getTiLEA_FormulaTipo()[3]);
       QString Formula3B_3=QString::fromStdString(MyLaserMP_Pr->getTiLEA_Formula()[3]);
       QString Formula3B_Unit_3=QString::fromStdString(MyLaserMP_Pr->getTiLEA_FormulaUnit()[3]);
       QString LEA_3B_3=QString::number(MyLaserMP_Pr->getTiLEA_Corrected()[3], 'e', 2);
       QString Formula3B_Label_3=QString("%1=%2 C<sub>5</sub> %3")
                               .arg(Formula3B_Tipo_3)
                               .arg(Formula3B_3)
                               .arg(Formula3B_Unit_3);

       QString LEA_3B_Label_3=QString("%1=%2 %3")
                               .arg(Formula3B_Tipo_3)
                               .arg(LEA_3B_3)
                               .arg(Formula3R_Unit_3);

       ui->Formula3B_Label_3->setText(Formula3B_Label_3);
       ui->LEA3B_label_3->setText(LEA_3B_Label_3);

       QString PowerErg3B_Cond_1_3=QString::number(MyLaserMP_Pr->getTiPowerErg_Cond_1()[3], 'e', 2);
       QString PowerErg3B_Cond_3_3=QString::number(MyLaserMP_Pr->getTiPowerErg_Cond_3()[3], 'e', 2);

       QString PowerErg3B_Cond_1_Label_3=QString("%1=%2 C<sub>5</sub> %3")
                            .arg(Formula3B_Tipo_3)
                            .arg(PowerErg3B_Cond_1_3)
                            .arg(Formula3B_Unit_3);

       QString PowerErg3B_Cond_3_Label_3=QString("%1=%2 %3")
                            .arg(Formula3B_Tipo_3)
                            .arg(PowerErg3B_Cond_3_3)
                            .arg(Formula3B_Unit_3);

       ui->cond1LEA_3B_Label_3->setText(PowerErg3B_Cond_1_Label_3);
       ui->cond3LEA_3B_Label_3->setText(PowerErg3B_Cond_3_Label_3);

       QString tPowerErg3B_Cond_1_Label_3=QString("%1<sub>Acc 1</sub>").arg(Formula3B_Tipo_3);
       ui->tCond1LEA_3B_Label_3->setText(tPowerErg3B_Cond_1_Label_3);

       QString tPowerErg3B_Cond_3_Label_3=QString("%1<sub>Acc 3</sub>").arg(Formula3B_Tipo_3);
       ui->tCond3LEA_3B_Label_3->setText(tPowerErg3B_Cond_3_Label_3);


       ui->class_Label_3->setText(QString::fromStdString(MyLaserMP_Pr->getTiLaserClassAssigned()));
}

void DockLea::on_internalWaist_checkBox_toggled(bool checked)
{
    if(n_laser==0)
        MyLaserCW_Pr->setInternalWaist(checked);
    if(n_laser==1)
        MyLaserSP_Pr->setInternalWaist(checked);
    if(n_laser==2)
        MyLaserMP_Pr->setInternalWaist(checked);

    setWidgets();
}
