#ifndef TESTTRANSFORM_H
#define TESTTRANSFORM_H

#include <QObject>
#include <QVector3D>

class TestTransform : public QObject
{
    Q_OBJECT
public:
    explicit TestTransform(QObject *parent = 0);
    
private slots:
    void doNothingApply();
    void shiftApply();
    void reflectApply();
    void orthogonalApply();
    void rotateXYApply();
    void rotateXZApply();
    void rotateYZApply();
    void scaleApply();

private:
    QVector3D ox_;
    QVector3D oy_;
    QVector3D oz_;
};

#endif // TESTTRANSFORM_H
