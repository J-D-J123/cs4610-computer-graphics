#include "ViewWidget.h"
#include <QDebug>

ViewWidget::ViewWidget(QWidget *parent, Qt::WindowFlags f)
  : QOpenGLWidget(parent, f)
{
}

void ViewWidget::initializeGL()
{
  initializeOpenGLFunctions();

  qDebug("OpenGL version %d.%d in ViewWidget constructor.",
    format().majorVersion(), format().minorVersion());
}

void ViewWidget::paintGL()
{
}

void ViewWidget::resizeGL(int w, int h)
{
}
