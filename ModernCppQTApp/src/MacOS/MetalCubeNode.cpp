#include "MetalCubeNode.h"
#include <QtQuick/private/qsgmetal_p.h>
#include <QDebug>

MetalCubeNode::MetalCubeNode(QQuickWindow* w)
    : m_window(w)
    , m_angle(0.0f)
{
    m_timer.start();
    setFlag(OwnsMaterial);
    setFlag(OwnsGeometry);
}

void MetalCubeNode::render(const RenderState* /*state*/)
{
    if (!m_window) return;

    auto* ri = m_window->rendererInterface();
    if (!ri || ri->graphicsApi() != QSGRendererInterface::Metal) {
        qWarning() << "Not running on Metal backend!";
        return;
    }

    void* renderEncoder = ri->metalRenderCommandEncoder();
    if (!renderEncoder) return;

    // Clear screen to red
    float color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    ri->metalClearRenderTarget(renderEncoder, color);

    // Update rotation angle
    qint64 elapsed = m_timer.elapsed();
    m_angle = fmod(elapsed / 16.0f, 360.0f);

    // TODO: Add Metal draw calls for cube vertices + shaders
    // This minimal example just clears to red
}
