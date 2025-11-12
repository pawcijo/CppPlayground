// MyQuickItem.h
#pragma once
#include <QQuickItem>

class MyQuickItem : public QQuickItem
{
    Q_OBJECT
public:
    MyQuickItem(QQuickItem* parent = nullptr);

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* data) override;
};
