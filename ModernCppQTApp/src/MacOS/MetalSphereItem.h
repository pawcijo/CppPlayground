#pragma once
#include <QQuickItem>

class MetalSphereItem : public QQuickItem
{
    Q_OBJECT
public:
    MetalSphereItem(QQuickItem* parent = nullptr);

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) override;
};