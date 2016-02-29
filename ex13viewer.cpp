//
//  Ex13viewer Widget
//

#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include "ex13viewer.h"

//
//  Constructor
//
Ex13viewer::Ex13viewer()
{
   //  Set window title
   setWindowTitle(tr("HW7:  Image Processing [D. Southard]"));

   //  Create new OpenGL widget
   Ex13opengl* ogl = new Ex13opengl;

   //  Select shader
   QComboBox* shader = new QComboBox();
   shader->addItem("Cats");
   shader->addItem("Dogs");
   shader->addItem("Transition");
   shader->addItem("Absolute difference");
   shader->addItem("False Color");
   shader->addItem("Rectangular Pixelation on Cats");
   shader->addItem("Hexagon Pixelation on Dogs");

   // Fraction
   flab = new QLabel("Fraction");
   frac = new QSlider(Qt::Horizontal);
   frac->setRange(0,100);
   frac->setValue(0);
   flab->hide();
   frac->hide();

   //  Reset
   QPushButton* rst = new QPushButton("Reset");
   //  Quit
   QPushButton* quit = new QPushButton("Quit");

   //  Set layout of child widgets
   QGridLayout* layout = new QGridLayout;
   layout->addWidget(ogl,0,0,4,1);
   layout->addWidget(new QLabel("Shader"),0,1);
   layout->addWidget(shader,0,2);
   layout->addWidget(flab,1,1);
   layout->addWidget(frac,1,2);
   layout->addWidget(rst,3,1);
   layout->addWidget(quit,3,2);
   //  Manage resizing
   layout->setColumnStretch(0,100);
   layout->setColumnMinimumWidth(0,100);
   layout->setRowStretch(2,100);
   setLayout(layout);

   //  Connect valueChanged() signals to ogl
   connect(shader,SIGNAL(currentIndexChanged(int)) , ogl,SLOT(setShader(int)));
   connect(frac,SIGNAL(valueChanged(int))          , ogl,SLOT(setFrac(int)));
   connect(shader,SIGNAL(currentIndexChanged(int)) , this,SLOT(setShader(int)));
   //  Connect reset() and lmove() signals
   connect(rst  ,SIGNAL(pressed()),ogl,SLOT(reset()));
   //  Connect quit() signal to qApp::quit()
   connect(quit,SIGNAL(pressed()) , qApp,SLOT(quit()));
}

//
//  Shader adjust
//
void Ex13viewer::setShader(int mode)
{
   if (mode==2 || mode == 5 || mode == 6)
   {
  	  if (mode != 2) flab->setText("Pixal Size");
  	  else flab->setText("Fraction");
      flab->show();
      frac->show();
   }
   else
   {
      flab->hide();
      frac->hide();
   }
}
