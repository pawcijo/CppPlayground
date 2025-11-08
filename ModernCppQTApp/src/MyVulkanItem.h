// MyVulkanItem.h
#pragma once
#include <QQuickItem>
#include <QVulkanInstance>
#include <QVulkanWindow>
#include <QSGSimpleRectNode>

class MyVulkanItem : public QQuickItem
{
    Q_OBJECT
public:
    explicit MyVulkanItem(QQuickItem* parent = nullptr)
        : QQuickItem(parent)
    {
        setFlag(QQuickItem::ItemHasContents, true);
    }

    void setVulkanInstance(QVulkanInstance* instance)
    {
        m_vkInstance = instance;
        // if you want, trigger an update
        update();
    }

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) override
    {
        // For now, just a colored rectangle so it is visible
        QSGSimpleRectNode* rectNode = static_cast<QSGSimpleRectNode*>(oldNode);
        if (!rectNode)
            rectNode = new QSGSimpleRectNode();

        rectNode->setRect(0, 0, width(), height());
        rectNode->setColor(QColor(100, 150, 250));  // temporary visible color

        return rectNode;
    }

private:
    QVulkanInstance* m_vkInstance = nullptr;
};
