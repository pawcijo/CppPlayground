// MetalItem.h
#pragma once
#include <QQuickItem>

class MetalItem : public QQuickItem
{
    Q_OBJECT
public:
    MetalItem(QQuickItem* parent = nullptr);

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* data) override;
};
