#include "ViewWidget.h"
#include <QDebug>

ViewWidget::ViewWidget(QWidget *parent, Qt::WindowFlags f)
  : QOpenGLWidget(parent, f)
{
}

void ViewWidget::initializeGL()
{
  initializeOpenGLFunctions();
}

void ViewWidget::paintGL()
{
}

void ViewWidget::resizeGL(int w, int h)
{
}
