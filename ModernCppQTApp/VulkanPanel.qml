import QtQuick 2.15
import MyVulkan 1.0

Item {
    anchors.fill: parent

    VulkanItem {
        id: vulkanItem
        objectName: "vulkanItem"
        anchors.fill: parent
    }
}
