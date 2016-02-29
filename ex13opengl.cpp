//
//  OpenGL widget
//
#include "ex13opengl.h"
#include <QtOpenGL>
#include <QMessageBox>
#include <math.h>

//
//  Constructor
//
Ex13opengl::Ex13opengl(QWidget* parent)
    : QGLWidget(parent)
{
   mode = 0;
   init  = false;
   mouse = false;
   x0 = y0 = 0;
   zoom = 1;
}

//
//  Reset view
//
void Ex13opengl::reset()
{
   x0 = y0 = 0;
   zoom = 1;
   frac = 0;
   //  Request redisplay
   updateGL();
}

//
//  Set shader
//
void Ex13opengl::setFrac(int pct)
{
   frac = pct;
   //  Request redisplay
   updateGL();
}

//
//  Set shader
//
void Ex13opengl::setShader(int sel)
{
   mode = sel;
   //  Request redisplay
   updateGL();
}

//
//  Load image to texture unit
//
void Ex13opengl::LoadImage(int unit,const QString file)
{
   //  Load image
   QImage img(file);
   //  Select texture unit
   glActiveTexture(unit);
   //  Bind texture
   unsigned int tex;
   glGenTextures(1,&tex);
   glBindTexture(GL_TEXTURE_2D,tex);
   //  Copy image to texture
   QImage rgba = QGLWidget::convertToGLFormat(img);
   w = rgba.width();
   h = rgba.height();
   glTexImage2D(GL_TEXTURE_2D,0,4,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,rgba.bits());
   //  Set pixel interpolation
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
}

//
//  Initialize
//
void Ex13opengl::initializeGL()
{
   if (init) return;
   init = true;

   //  Load shader
   Shader(shader,"",":/ex13.frag");

   //  Load images
   LoadImage(GL_TEXTURE0,":/cats.jpg");
   LoadImage(GL_TEXTURE1,":/dogs.jpg");
}

//
//  Set projection
//
void Ex13opengl::resizeGL(int width, int height)
{
   //  Window aspect ratio
   float asp = width/(float)height;
   //  Viewport is entire window
   glViewport(0,0,width,height);
   //  Set Projection
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-asp, +asp, -1, +1, -1, +1);
   //  Set Projection
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

//
//  Draw the window
//
void Ex13opengl::paintGL()
{
   //  Image aspect ratio
   float asp = w / (float)h;

   //  Enable shader
   shader.bind();

   //  Set pixel increments
   shader.setUniformValue("img0",0);
   shader.setUniformValue("img1",1);
   shader.setUniformValue("frac",(float)(frac/100.0));
   shader.setUniformValue("mode",mode);

   //  Set ModelView
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glScaled(zoom,zoom,1);
   glTranslated(x0,y0,0);
   //  Draw to screen
   glClear(GL_COLOR_BUFFER_BIT);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex2f(-asp,-1);
   glTexCoord2f(1,0); glVertex2f(+asp,-1);
   glTexCoord2f(1,1); glVertex2f(+asp,+1);
   glTexCoord2f(0,1); glVertex2f(-asp,+1);
   glEnd();

   //  Done with shader
   shader.release();
}

//
//  Throw a fatal error and die
//
void Ex13opengl::Fatal(QString message)
{
   QMessageBox::critical(this,"Ex13opengl",message);
   QApplication::quit();
}

/******************************************************************/
/*************************  Mouse Events  *************************/
/******************************************************************/
//
//  Mouse pressed
//
void Ex13opengl::mousePressEvent(QMouseEvent* e)
{
   mouse = true;
   pos = e->pos();  //  Remember mouse location
}

//
//  Mouse released
//
void Ex13opengl::mouseReleaseEvent(QMouseEvent*)
{
    mouse = false;
}

//
//  Mouse moved
//
void Ex13opengl::mouseMoveEvent(QMouseEvent* e)
{
   if (mouse)
   {
      QPoint d = e->pos()-pos;      //  Change in mouse location
      x0 += d.x()/(zoom*width());   //  Translate x movement to azimuth
      y0 -= d.y()/(zoom*height());  //  Translate y movement to elevation
      pos = e->pos();               //  Remember new location
      updateGL();                   //  Request redisplay
   }
}

//
//  Mouse wheel
//
void Ex13opengl::wheelEvent(QWheelEvent* e)
{
   //  Zoom out
   if (e->delta()<0)
      zoom *= 1.05;
   //  Zoom in
   else if (zoom>1)
      zoom /= 1.05;
   //  Request redisplay
   updateGL();
}

//
//  Load shader
//
void Ex13opengl::Shader(QGLShaderProgram& shader,QString vert,QString frag)
{
   //  Vertex shader
   if (vert.length() && !shader.addShaderFromSourceFile(QGLShader::Vertex,vert))
      Fatal("Error compiling "+vert+"\n"+shader.log());
   //  Fragment shader
   if (frag.length() && !shader.addShaderFromSourceFile(QGLShader::Fragment,frag))
      Fatal("Error compiling "+frag+"\n"+shader.log());
   //  Link
   if (!shader.link())
      Fatal("Error linking shader\n"+shader.log());
}
