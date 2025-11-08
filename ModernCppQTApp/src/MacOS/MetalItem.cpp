// MetalItem.cpp
#include "MetalItem.h"
#include <QSGSimpleTextureNode>
#include <QQuickWindow>
#include <QSGRendererInterface>
#include <QDebug>
#include <QSGFlatColorMaterial>

MetalItem::MetalItem(QQuickItem* parent)
    : QQuickItem(parent)
{
    // Tell Qt Quick this item will use a custom scene graph node
    setFlag(ItemHasContents, true);
}

QSGNode* MetalItem::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*)
{
    if (!window()) return nullptr;

    QSGNode* node = oldNode;
    if (!node) {
        node = new QSGNode();
    }

    // Check the renderer backend
    QSGRendererInterface::GraphicsApi api = window()->rendererInterface()->graphicsApi();
    if (api != QSGRendererInterface::Metal) {
        qWarning() << "Warning: Not running on Metal! Current backend:" << api;
    } else {
        qDebug() << "Using Metal backend!";
    }

    // Clear with red color
    // On Metal, we don’t call glClearColor; instead we can use a QSGSimpleRectNode with red
    // Or create a custom QSGGeometryNode if you want 3D
    QSGGeometryNode* geometryNode = new QSGGeometryNode();
    QSGGeometry* geom = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 4);
    geom->setDrawingMode(GL_TRIANGLE_STRIP);
    geometryNode->setGeometry(geom);
    geometryNode->setFlag(QSGNode::OwnsGeometry);

    QSGFlatColorMaterial* material = new QSGFlatColorMaterial();
    material->setColor(Qt::red);
    geometryNode->setMaterial(material);
    geometryNode->setFlag(QSGNode::OwnsMaterial);

    // Define rectangle covering the item
    QSGGeometry::Point2D* vertices = geom->vertexDataAsPoint2D();
    QSizeF s = size();
    vertices[0].set(0, 0);
    vertices[1].set(s.width(), 0);
    vertices[2].set(0, s.height());
    vertices[3].set(s.width(), s.height());

    return geometryNode;
}
