#include "MetalNodeItem.h"
#include "MetalRenderNode.h"

MetalNodeItem::MetalNodeItem(QQuickItem* parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
}

QSGNode* MetalNodeItem::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*)
{
    MetalRenderNode* node = static_cast<MetalRenderNode*>(oldNode);
    if (!node)
        node = new MetalRenderNode();

    update(); // schedule redraw
    return node;
}
