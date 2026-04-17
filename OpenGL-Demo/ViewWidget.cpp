#include "ViewWidget.h"
#include <QDebug>

ViewWidget::ViewWidget(QWidget *parent, Qt::WindowFlags f)
  : QOpenGLWidget(parent, f)
{

    qDebug("OpenGL version %d.%d in ViewWidget constructor.",
           format().majorVersion(), format().minorVersion());
}

void ViewWidget::initializeGL()
{
    // // question 2 gl clear color
    // glClearColor(0.15, 0.15, 0.15, 1.0); // crashes

    initializeOpenGLFunctions();

    // question 2 gl clear color
    glClearColor(0.15, 0.15, 0.15, 1.0);

    // question 1
    qDebug("OpenGL version %d.%d in ViewWidget initializeGL().",
        format().majorVersion(), format().minorVersion());

    // question 4
    // create the out VBO and load with some vertex data
    GLfloat triangle[] = {
        -1.0f, -0.666667f, 0.0f,   1.0f, 0.0f, 0.0f,
        1.0f, -0.166667f, 0.0f,   0.0f, 1.0f, 0.0f,
        0.0f,  0.833333f, 0.0f,   0.0f, 0.0f, 1.0f
    };

    m_vbo.create();
    m_vbo.bind();
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vbo.allocate(triangle, sizeof(triangle));
    m_vbo.release();

    // question 5
    // compiles and links together the shaders
    const char *vertexSource = R"(
        #version 330

        in vec3 rgb;
           in vec3 xyz;

           uniform mat4 mvp;

           out vec3 color;

           void main()
           {
             gl_Position = mvp * vec4(xyz, 1.0);
             color = rgb;
           }

    )";

    const char *fragmentSource = R"(
        #version 330

        in vec3 color;
        out vec4 fragColor;

        void main()
        {
            fragColor = vec4(color, 1.0);
        }
    )";

    m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexSource);
    m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentSource);
    m_program.link();

    // binds the VBO and binds the VAO for limited time
    // which allows the pointers to point into the VBO memory
    m_vbo.bind();
    {
        QOpenGLVertexArrayObject::Binder bind(&m_vao);

        m_program.enableAttributeArray("xyz");
        m_program.enableAttributeArray("rgb");

        m_program.setAttributeBuffer("xyz", GL_FLOAT, 0, 3, 6 * sizeof(GLfloat));
        m_program.setAttributeBuffer("rgb", GL_FLOAT, 3 * sizeof(GLfloat), 3,
                                     6 * sizeof(GLfloat));
    }
    m_vbo.release();
}

void ViewWidget::paintGL()
{

    // question 8
    QMatrix4x4 projection;
    QMatrix4x4 modelview;

    // added to test scales
    // modelview.scale(1.0, -1.0, 1.0);

    modelview.rotate(45, {0.0, 0.0, 1.0});

    // tells the program to draw the data we set up with the VAO
    // binds the program so it will be used for drawing, and binding the VAO
    // which means data will be pulled from the configuration we created above
    // starts with first(0) vertex, and draws 3 of them
    m_program.bind();
    {
        // flips/ translates etc.. .
        m_program.setUniformValue("mvp", projection * modelview);

        QOpenGLVertexArrayObject::Binder bind(&m_vao);
        // glPointSize(24.0f);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    m_program.release();
}

void ViewWidget::resizeGL(int w, int h)
{
}
