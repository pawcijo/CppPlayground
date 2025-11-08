#pragma once
#include <QSGRenderNode>
#include <QElapsedTimer>
#include <QMatrix4x4>
#include <QQuickWindow>

class MetalCubeNode : public QSGRenderNode
{
public:
    explicit MetalCubeNode(QQuickWindow* w);
    ~MetalCubeNode() override = default;

    QSGRenderNode::StateFlags changedStates() const override {
        return QSGRenderNode::DepthState | QSGRenderNode::BlendState;
    }

    void render(const RenderState* state) override;

private:
    QQuickWindow* m_window;
    float m_angle;
    QElapsedTimer m_timer;
};
