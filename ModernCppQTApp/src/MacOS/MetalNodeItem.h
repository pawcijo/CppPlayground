#pragma once
#include <QQuickItem>

class MetalNodeItem : public QQuickItem
{
    Q_OBJECT
public:
    MetalNodeItem(QQuickItem* parent = nullptr);

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) override;
};
