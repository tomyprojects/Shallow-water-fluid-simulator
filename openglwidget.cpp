#include "openglwidget.h"
#include "exception.h"
#include "rendershallowwater.h"
#include "testvec2.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) :
    QGLWidget(parent),
    upPressed(false), downPressed(false), leftPressed(false), rightPressed(false), kPressed(false), mPressed(false)
{
    QTimer * timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer->setInterval(40);
    timer->start();

    setFocusPolicy(Qt::StrongFocus);

    qDebug() << "OpenGLWidget";
}

OpenGLWidget::~OpenGLWidget()
{
}

void OpenGLWidget::sphere(float r, float x=0, float y=0, float z=0)
{
    glPushMatrix();

    glTranslatef(x, y, z);

    GLUquadricObj * quadric = gluNewQuadric();

    gluSphere(quadric, (GLdouble)r, 20, 20);

    gluDeleteQuadric(quadric);

    glPopMatrix();
}

void OpenGLWidget::triangle(float l, float x=0, float y=0, float z=0)
{
    glBegin(GL_TRIANGLES);
    glVertex3f(x,y,z);
    glVertex3f(x+l,y,z);
    glVertex3f(x,y+l,z);
    glEnd();
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    cameraPosition();

    msim.update(9.8);

    //msim.setWaterLevel(100,100,1.0);

    //sphere(1);
    glPushMatrix();
    glTranslatef(-64*1/2, 0, -64*1/2);
    //RenderHeightMap(hmap,1,1,10).triangles();
    glColor3f(1,0,0);
    RenderShallowWater(msim, 1, 1, 1).lines(6.0);
    glColor3f(1,1,1);
    RenderShallowWater(msim, 1, 1, 1).points();
    glPopMatrix();

    //sphere(0.2,2,0,0);
}

void OpenGLWidget::resizeGL(int width, int height)
{
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    glFrustum(-1,1,-1,1,1,100);
}

void OpenGLWidget::initializeGL()
{

    qDebug() << "Opengl: " << QString((char *)glGetString(GL_VERSION));

    glEnable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);
    TestVec2 testvec2;
    //glEnable(GL_LIGHTING);

    //glEnable(GL_LIGHT0);

    try
    {
        //hmap.loadFromBmp("/Users/tyeung/Desktop/FluidSolver/fractal2.bmp");
        msim.loadTerrainHeightmap("/Users/Jim/Desktop/Google Drive/Projects/FluidSolver/fractal2.bmp", 10);
        msim.loadWaterDepthmap("/Users/Jim/Desktop/Google Drive/Projects/FluidSolver/fractal2.bmp", 1);
    } catch (my::Exception e)
    {
        qDebug() << e.msg();
        exit(1);
    }
}

void OpenGLWidget::keyPressEvent(QKeyEvent * event)
{
    switch (event->key())
    {
    case Qt::Key_Down:
        downPressed = true;
        break;

    case Qt::Key_Left:
        leftPressed = true;
        break;

    case Qt::Key_Right:
        rightPressed = true;
        break;

    case Qt::Key_Up:
        upPressed = true;
        break;
    case Qt::Key_K:
        kPressed = true;
        break;
    case Qt::Key_M:
        mPressed = true;
        break;
    }

    QGLWidget::keyPressEvent(event);
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent * event)
{
    switch (event->key())
    {
    case Qt::Key_Down:
        downPressed = false;
        break;

    case Qt::Key_Left:
        leftPressed = false;
        break;

    case Qt::Key_Right:
        rightPressed = false;
        break;

    case Qt::Key_Up:
        upPressed = false;
        break;
    case Qt::Key_K:
        kPressed = false;
        break;
    case Qt::Key_M:
        mPressed = false;
        break;
    }

    QGLWidget::keyReleaseEvent(event);
}

void OpenGLWidget::cameraPosition()
{
    typedef qreal Radian;
    typedef qreal Meter;

    static Meter x;
    static Meter y = 24;
    static Meter z;

    static Meter radius = 18;
    static Radian radians = 1.57;

    static const Meter forwardStep = 0.2;
    static const Radian sideStep = 0.1;
    static const Meter upStep = 0.2;


    // update forward and side values
    Meter forward = int(upPressed) - int(downPressed);
    Radian side = int(rightPressed) - int(leftPressed);

    forward *= forwardStep;
    side *= sideStep;

    // update radius and radians values
    radius -= forward;
    radians -= side;

    // define x, y, and z values
    x = radius * qCos(radians);
    y += upStep * int(kPressed);
    y -= upStep * int(mPressed);
    z = radius * qSin(radians);


    gluLookAt(x,y,z,0,0,0,0,1,0);
}
