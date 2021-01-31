#include "sliderscrolllabel.h"
#include <cmath>
#include <QDebug>

SliderScrollLabel::SliderScrollLabel(QWidget *parent) :
    QWidget(parent)
{    
    scientNotLabelStyle="QLabel {background-color: #fafafa}\n"
                               "QLabel {border: 1px solid grey}\n"
                               "QLabel {border-radius: 8px}\n"
                               "QLabel {color: #000000}\n"
                               "QLabel {padding-left: 5px\n}"
                               "QLabel {padding-right: 5px}";


    scientNotLabelStyleOff="QLabel {background-color: #f0f0f0}\n"
                                  "QLabel {border: 1px solid #a0a0a0}\n"
                                  "QLabel {border-radius: 8px}\n"
                                  "QLabel {color: #a0a0a0}\n"
                                  "QLabel {padding-left: 5px\n}"
                                  "QLabel {padding-right: 5px}";



        setMaximumSize(QSize(220, 75));
        gridLayout = new QGridLayout(this);
        gridLayout->setObjectName(tr("gridLayout"));

        titleLabel = new QLabel(this);
        titleLabel->setObjectName(tr("titleLabel"));
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet(tr("QLabel {background-color: #00c800}"
                                     "QLabel {color: #fafafa}"
                                     "QLabel {border-radius: 8px}"
                                     "QLabel {border: 0px}"
                                     "QLabel {padding: 5px}"
                                     "QLabel {margin-left: 55px}"
                                     "QLabel {margin-right: 55px}"));


        gridLayout->addWidget(titleLabel, 0, 1, 1, 3);

        slider = new QSlider(this);
        slider->setObjectName(tr("slider"));
        slider->setMinimum(1);
        slider->setMaximum(999);
        slider->setObjectName(tr("slider"));
        slider->setOrientation(Qt::Horizontal);
        slider->setStyleSheet(tr("QSlider::handle:horizontal {background-color: #e0e0e0}\n"
                                 "QSlider::sub-page:horizontal{background: #00c800}"));

        gridLayout->addWidget(slider, 1, 1, 1, 1);

        scientNotLabel = new QLabel(this);
        scientNotLabel->setObjectName(tr("title"));
        scientNotLabel->setAlignment(Qt::AlignCenter);
        scientNotLabel->setStyleSheet(scientNotLabelStyle);


        gridLayout->addWidget(scientNotLabel, 1, 2, 1, 1);

        scrollBar = new QScrollBar(this);
        scrollBar->setObjectName(tr("scrollBar"));
        scrollBar->setMinimumSize(QSize(0, 25));
        scrollBar->setOrientation(Qt::Vertical);
        scrollBar->setMinimum(-13);
        scrollBar->setMaximum(11);
        scrollBar->setValue(0);
        scrollBar->setSliderPosition(0);

        gridLayout->addWidget(scrollBar, 1, 3, 1, 1);

        slider->setValue(10);
        scrollBar->setValue(-1);

        mantissa=0.1;
        exponent=1;
        setScientificNumber();

        QFont font;
        font.setPointSize(6);
        titleLabel->setFont(font);
        scientNotLabel->setFont(font);

        connect(slider, SIGNAL(valueChanged(int)), this, SLOT(on_slider_valueChanged(int)));
        connect(scrollBar, SIGNAL(valueChanged(int)), this, SLOT(on_scrollBar_valueChanged(int)));
}

SliderScrollLabel::~SliderScrollLabel()
{

}

void SliderScrollLabel::on_slider_valueChanged(int value)
{
    mantissa= value/100.0;
    setScientificNumber();
    emit valueChanged(value);
}

void SliderScrollLabel::on_scrollBar_valueChanged(int value)
{
    exponent=-value;
    setScientificNumber();
    emit valueChanged(value);
}

void SliderScrollLabel::setScientificNumber()
{
    scientificNumber=mantissa*powf(10, exponent);
    scientNotLabel->setText(QString::number(scientificNumber,'e', 2));
}

void SliderScrollLabel::setValue(const double& _value)
{
    double value=_value;
    scientNotLabel->setText(QString::number(value, 'e', 2));
    mantissa=QString::number(value, 'e', 2).left(4).toDouble();
    qDebug()<< "Mantissa: " << mantissa;
    slider->setValue((int)(mantissa*100));

    if(value>=1)
        exponent=+QString::number(value, 'e', 2).right(2).toInt();
        else
        exponent=-(QString::number(value, 'e', 2).right(2).toInt());

    scrollBar->setValue(-exponent);

    scientificNumber=mantissa*powf(10, exponent);
}

double SliderScrollLabel::getScientificNumber()const
{
    return scientificNumber;
}

/**********************************************************************************
 * La sliderBar verticale incrementa i valori con il tasto rivolto verso il basso *
 * decrementa i valori con il tasto rivolto verso l'alto. Affinchè il controllo   *
 * funzioni come uno spinBox (decrementndo i valori con il tasto rivolto verso    *
 * l'alto e decrementando i valori azionando il tasto rivolto verso il basso) è   *
 * necessario invertire il segno del valore della sliderBar. In questo modo il    *
 * valore minimo diventa il massimo e viceversa il massimo diventa minimo.        *
 **********************************************************************************/

void SliderScrollLabel::setMaximumExponent(int value)
{
    scrollBar->setMinimum(-value);
}

int SliderScrollLabel::getMaximumExponent()const
{
    return -scrollBar->minimum();
}

void SliderScrollLabel::setMinimumExponent(int value)
{
    scrollBar->setMaximum(-value);
}

int SliderScrollLabel::getMinimumExponent()const
{
    return -scrollBar->maximum();
}

void SliderScrollLabel::setTitle(const QString _title)
{
    titleLabel->setText(_title);
}

QString SliderScrollLabel::getTitle()const
{
    return titleLabel->text();
}

int SliderScrollLabel::getExponent()const
{
    return exponent;
}

double SliderScrollLabel::getMantissa()const
{
    return mantissa;
}

void SliderScrollLabel::setExponent(const int _exponent)
{
    scrollBar->setValue(-_exponent);
    exponent=_exponent;
}

void SliderScrollLabel::setMantissa(const double _mantissa)
{
    slider->setValue((int)(_mantissa*100));
    mantissa=_mantissa;
}

void SliderScrollLabel::setEnabled(bool _enabled)
{
    {
        slider->setEnabled(_enabled);
        scrollBar->setEnabled(_enabled);

        if(_enabled)
        {
        scientNotLabel->setStyleSheet(scientNotLabelStyle);
        }
        else
        {
        scientNotLabel->setStyleSheet(scientNotLabelStyleOff);
        }
    }
}

