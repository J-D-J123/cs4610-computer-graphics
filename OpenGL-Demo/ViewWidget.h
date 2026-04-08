#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class ViewWidget : public QOpenGLWidget, QOpenGLFunctions
{
  Q_OBJECT

public:
  explicit ViewWidget(QWidget *parent = nullptr,
                      Qt::WindowFlags f = Qt::WindowFlags());

protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;

private:

};
#endif // VIEWWIDGET_H

