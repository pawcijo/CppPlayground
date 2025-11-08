#pragma once
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#include <QQuickWindow>
#include <QMatrix4x4>
#include <QElapsedTimer>

class MetalSphereNode : public QSGGeometryNode
{
public:
    MetalSphereNode();
    void updateGeometry();

private:
    QSGGeometry* m_geometry;
    QSGFlatColorMaterial* m_material;
    float m_angle;
    QElapsedTimer m_timer;
};