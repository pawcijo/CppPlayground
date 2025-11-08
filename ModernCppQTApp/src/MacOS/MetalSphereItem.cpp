#include "MetalSphereItem.h"
#include "MetalSphereNode.h"

MetalSphereItem::MetalSphereItem(QQuickItem* parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
}

QSGNode* MetalSphereItem::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*)
{
    MetalSphereNode* node = static_cast<MetalSphereNode*>(oldNode);
    if (!node)
        node = new MetalSphereNode();

    // schedule redraw
    update();

    return node;
}
