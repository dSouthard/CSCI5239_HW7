#ifndef EX13V_H
#define EX13V_H

#include <QWidget>
#include <QSlider>
#include <QDoubleSpinBox>
#include "ex13opengl.h"

class Ex13viewer : public QWidget
{
Q_OBJECT
public:
    Ex13viewer();
    QLabel*  flab;
    QSlider* frac;
private slots:
    void setShader(int mode);
};

#endif
