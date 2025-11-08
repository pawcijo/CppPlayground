#pragma once
#include <QSGRenderNode>
#include <QMatrix4x4>
#include <QElapsedTimer>
#include <QQuickWindow>

class MetalRenderNode : public QSGRenderNode
{
public:
    MetalRenderNode(QQuickWindow* w)
        : m_window(w), m_angle(0.0f)
    {
        m_timer.start();
        setFlag(OwnsMaterial);
        setFlag(OwnsGeometry);
    }

    void render(const RenderState* state) override;

private:
    QQuickWindow* m_window;
    float m_angle;
    QElapsedTimer m_timer;
};
