#include "glwidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWheelEvent>


GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      displayMode(GL_TRIANGLES),
      xRot(0), yRot(0), zRot(0),
      camera(QVector3D(2,2,5), QVector3D(0,1,0)),
      culling(false), testing(true)
{
}

void GLWidget::open(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Obj Files (*.obj);"));
    qDebug() << "---open()---";

    if(fileName != ""){
        models.push_back(new Model(fileName));
        //models[modelCount]->setTexture(QString("E:/3D Objects/assets/chest/diffuse.webp"));
        //emit loadedMesh(models[modelCount]->getName());
        emit loadedModel(models[modelCount]);

        modelCount++;
        update();
    }
}

void GLWidget::openTexture(){
    QString fileName = QFileDialog::getOpenFileName(this);
    qDebug() << "---openTexture()---";

    if(fileName != ""){
        if(selectedModelIndex != -1)
        {
            models[selectedModelIndex]->setTexture(fileName);
            //emit loadedTexture(fileName);
            update();
        }
    }
}

void GLWidget::selectedModel(QModelIndex modelIndex){
    selectedModelIndex = modelIndex.row();
    qDebug() << "Select:" << selectedModelIndex;

    emit setSpinboxPositionX(models[selectedModelIndex]->getPosition().x());
    emit setSpinboxPositionY(models[selectedModelIndex]->getPosition().y());
    emit setSpinboxPositionZ(models[selectedModelIndex]->getPosition().z());
    emit setSpinboxScaleX(models[selectedModelIndex]->getScale().x());
    emit setSpinboxScaleY(models[selectedModelIndex]->getScale().y());
    emit setSpinboxScaleZ(models[selectedModelIndex]->getScale().z());
    emit setSpinboxRotationX(models[selectedModelIndex]->getRotation().x());
    emit setSpinboxRotationY(models[selectedModelIndex]->getRotation().y());
    emit setSpinboxRotationZ(models[selectedModelIndex]->getRotation().z());
}

void GLWidget::initializeGL(){
    initializeOpenGLFunctions();
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    shader.init();

    grid.init();
    modelMatrix.setToIdentity();
    frame = 0;
}


void GLWidget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT);

    if(culling) glEnable(GL_CULL_FACE);
    else        glDisable(GL_CULL_FACE);
    if(testing) glEnable(GL_DEPTH_TEST);
    else        glDisable(GL_DEPTH_TEST);

    camera.transform(xRot, yRot);

    modelMatrix.setToIdentity();
    shader.update(proj, camera.matrix, modelMatrix);

    // Draw Grid
    grid.draw(shader.program);

    // Draw Models
    for(int i = 0; i < modelCount; i++){
        modelMatrix.setToIdentity();
        modelMatrix.translate(models[i]->getPosition());
        modelMatrix.rotate(QQuaternion::fromEulerAngles(models[i]->getRotation()));
        modelMatrix.scale(models[i]->getScale());

        shader.update(proj, camera.matrix, modelMatrix);
        models[i]->draw(shader.program, displayMode);
    }

    qDebug() << ++frame;
}


///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


void GLWidget::resizeGL(int w, int h){
    proj.setToIdentity();
    proj.perspective(45.0f, GLfloat(w)/h, 0.01f, 1000.0f);
}

// Window Size
QSize GLWidget::minimumSizeHint() const{return QSize(600, 400);}
QSize GLWidget::sizeHint() const{return QSize(600, 400);}

// Mouse Event
void GLWidget::mousePressEvent(QMouseEvent *event){
    lastPos = event->pos();
}
void GLWidget::mouseMoveEvent(QMouseEvent *event){
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 4 * dy);
        setYRotation(yRot + 4 * dx);
    }
    lastPos = event->pos();
}
void GLWidget::wheelEvent(QWheelEvent *event){
    int degree = event->angleDelta().y() / 8;
    camera.dolly(degree);
    update();
}



// Slots
void GLWidget::setDisplayMode(bool arg){
    if(arg){displayMode = GL_LINE_STRIP;}
    else{displayMode = GL_TRIANGLES;}
    update();
}
void GLWidget::setCullFace(bool arg){
    culling = arg;
    update();
}
void GLWidget::setDepthTest(bool arg){
    testing = arg;
    update();
}

void GLWidget::changeModelPositionX(double x){models[selectedModelIndex]->setPositionX(x);update();}
void GLWidget::changeModelPositionY(double y){models[selectedModelIndex]->setPositionY(y);update();}
void GLWidget::changeModelPositionZ(double z){models[selectedModelIndex]->setPositionZ(z);update();}
void GLWidget::changeModelScaleX(double x){models[selectedModelIndex]->setScaleX(x);update();}
void GLWidget::changeModelScaleY(double y){models[selectedModelIndex]->setScaleY(y);update();}
void GLWidget::changeModelScaleZ(double z){models[selectedModelIndex]->setScaleZ(z);update();}
void GLWidget::changeModelRotationX(double x){models[selectedModelIndex]->setRotationX(x);update();}
void GLWidget::changeModelRotationY(double y){models[selectedModelIndex]->setRotationY(y);update();}
void GLWidget::changeModelRotationZ(double z){models[selectedModelIndex]->setRotationZ(z);update();}


static void qNormalizeAngle(int &angle){
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}
void GLWidget::setXRotation(int angle){
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        //emit xRotationChanged(angle);
        update();
    }
}
void GLWidget::setYRotation(int angle){
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        //emit yRotationChanged(angle);
        update();
    }
}
void GLWidget::setZRotation(int angle){
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        //emit zRotationChanged(angle);
        update();
    }
}
