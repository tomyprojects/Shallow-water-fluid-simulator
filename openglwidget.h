#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QWidget>
#include <QtOpenGL>
#ifdef MAC_OS_X_VERSION_10_7
#include <glu.h>
#else
#include <gl/GLU.h>
#endif
#include "shallowwater.h"
#include "vec2.h"
#include "heightmap.h"
#include "renderheightmap.h"

class OpenGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget *parent = 0);
    ~OpenGLWidget();
signals:

public slots:


protected:
    void paintGL();
    void resizeGL(int width, int height);
    void initializeGL();

    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

private:
    typedef bool Key;
    typedef int Attribute;

    Key upPressed, downPressed, leftPressed, rightPressed;
    Key kPressed, mPressed;

    ShallowWater<64, 64> msim;
    HeightMap hmap;

    void sphere(float r, float x, float y, float z);
    void triangle(float l, float x, float y, float z);
    void cameraPosition();
};

#endif // OPENGLWIDGET_H
