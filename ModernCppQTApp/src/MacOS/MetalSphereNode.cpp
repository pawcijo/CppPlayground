#include "MetalSphereNode.h"
#include <QtMath>
#include <QDebug>
#include <QVector3D>
#include <OpenGL/gl.h>

MetalSphereNode::MetalSphereNode()
    : m_angle(0.0f)
{
    m_timer.start();

    // Create geometry for sphere (simple vertex-based)
    m_geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
    m_geometry->setDrawingMode(GL_TRIANGLES);
    setGeometry(m_geometry);
    setFlag(OwnsGeometry);

    m_material = new QSGFlatColorMaterial();
    m_material->setColor(Qt::red);
    setMaterial(m_material);
    setFlag(OwnsMaterial);

    updateGeometry();
}

void MetalSphereNode::updateGeometry()
{
    // Update rotation angle based on elapsed time
    qint64 elapsed = m_timer.elapsed();
    m_angle = (elapsed / 1000.0f) * 45.0;
    // Generate a simple UV sphere
    const int latSegments = 16;
    const int lonSegments = 32;
    QVector<QSGGeometry::Point2D> vertices;
    vertices.reserve(latSegments * lonSegments * 6);

    float radius = 0.5f;
    for (int i = 0; i < latSegments; ++i) {
        float theta1 = (float)i / latSegments * M_PI;
        float theta2 = (float)(i + 1) / latSegments * M_PI;

        for (int j = 0; j < lonSegments; ++j) {
            float phi1 = (float)j / lonSegments * 2 * M_PI;
            float phi2 = (float)(j + 1) / lonSegments * 2 * M_PI;

            // 4 vertices of quad
            QVector3D v1(radius * qSin(theta1) * qCos(phi1),
                         radius * qCos(theta1),
                         radius * qSin(theta1) * qSin(phi1));
            QVector3D v2(radius * qSin(theta2) * qCos(phi1),
                         radius * qCos(theta2),
                         radius * qSin(theta2) * qSin(phi1));
            QVector3D v3(radius * qSin(theta2) * qCos(phi2),
                         radius * qCos(theta2),
                         radius * qSin(theta2) * qSin(phi2));
            QVector3D v4(radius * qSin(theta1) * qCos(phi2),
                         radius * qCos(theta1),
                         radius * qSin(theta1) * qSin(phi2));

            // Two triangles
            vertices.append({v1.x(), v1.y()});
            vertices.append({v2.x(), v2.y()});
            vertices.append({v3.x(), v3.y()});

            vertices.append({v1.x(), v1.y()});
            vertices.append({v3.x(), v3.y()});
            vertices.append({v4.x(), v4.y()});
        }
    }

    m_geometry->allocate(vertices.size());
    auto* vertexData = m_geometry->vertexDataAsPoint2D();
    for (int i = 0; i < vertices.size(); ++i) {
        vertexData[i] = vertices[i];
    }

    markDirty(QSGNode::DirtyGeometry);
}
