#pragma once
#include <QVulkanWindow>


class SimpleRenderer : public QVulkanWindowRenderer {
public:
    SimpleRenderer(QVulkanWindow* window) : m_window(window) {}
    
    void initResources() override;

    void startNextFrame() override;

    void releaseResources() override;

private:
    QVulkanWindow* m_window;
};