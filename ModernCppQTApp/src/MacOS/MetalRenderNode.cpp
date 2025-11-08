#include "MetalRenderNode.h"
#include <QQuickWindow>
#include <QtQuick/private/qsgmetal_p.h>
#include <QtMath>
#include <QDebug>

MetalRenderNode::MetalRenderNode()
    : m_angle(0.0f)
{
    m_timer.start();
    setFlag(OwnsMaterial);
    setFlag(OwnsGeometry);
}

MetalRenderNode::~MetalRenderNode()
{
    // TODO: release Metal resources if allocated
}

void MetalRenderNode::render(const RenderState* state)
{
    Q_UNUSED(state);
    if (!state->window()) return;

    auto* ri = window()->rendererInterface();
    if (!ri || ri->graphicsApi() != QSGRendererInterface::Metal) {
        qWarning() << "Not running on Metal backend!";
        return;
    }

    // Get Metal command encoder
    void* renderEncoder = ri->metalRenderCommandEncoder();
    if (!renderEncoder) return;

    // --- Clear screen red ---
    float color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    ri->metalClearRenderTarget(renderEncoder, color);

    // --- Update rotation ---
    qint64 elapsed = m_timer.elapsed();
    m_angle = (elapsed / 16.0f); // ~1 degree per frame

    // TODO: Encode Metal draw commands for 3D cube or sphere here
    // You need vertex buffer, vertex shader, fragment shader, pipeline state

    // NOTE: This requires low-level Metal knowledge; Qt Quick exposes only
    // minimal access via QSGRendererInterface::metal*.
}
