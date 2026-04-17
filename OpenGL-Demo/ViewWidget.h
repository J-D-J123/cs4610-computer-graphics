#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

// question 4
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

// question 5
#include <QOpenGLShaderProgram>

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
  QOpenGLVertexArrayObject m_vao;
  QOpenGLBuffer m_vbo;

  // Question 5
  QOpenGLShaderProgram m_program;

};
#endif // VIEWWIDGET_H

