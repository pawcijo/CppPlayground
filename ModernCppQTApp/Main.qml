import QtQuick 6.6
import QtQuick.Window 6.6
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Extras 2.0
import Qt3D.Quick 2.0  

Window {
    visible: true
    width: 600
    height: 600
    title: qsTr("Qt 3D Rotating Red Cube")

    Scene3D {
        anchors.fill: parent
        aspects: ["render"]

        Entity {
            id: sceneRoot

            components: [
                RenderSettings {},
                InputSettings {}
            ]

            // Camera
            Camera {
                id: camera
                position: Qt.vector3d(0, 0, 600)
                viewCenter: Qt.vector3d(0, 0, 0)
                projectionType: CameraLens.PerspectiveProjection
            }

            // Directional light
            DirectionalLight {
                worldDirection: Qt.vector3d(-1, -1, -1)
                color: "white"
                intensity: 1
            }

            // Red Cube
            Entity {
                id: cubeEntity

                components: [
                    CubeMesh {},
                    PhongMaterial { diffuse: "red" },
                    Transform {
                        id: cubeTransform
                        scale: 100
                    }
                ]

                // Continuous rotation
                SequentialAnimation on cubeTransform.rotationY {
                    loops: Animation.Infinite
                    NumberAnimation { from: 0; to: 360; duration: 4000 }
                }
            }

            // Optional: Ground plane or sphere instead
            // Entity {
            //     components: [
            //         SphereMesh { radius: 50 },
            //         PhongMaterial { diffuse: "red" },
            //         Transform { translation: Qt.vector3d(0, 0, 0) }
            //     ]
            // }
        }
    }
}
