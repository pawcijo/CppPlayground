#pragma once

#include <QObject>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <QVariantAnimation>

#include <Mendelejew/Element.hpp>

class ElementVisualizer : public QObject
{
    Q_OBJECT

public:
    explicit ElementVisualizer(Qt3DCore::QEntity* rootEntity, QObject* parent = nullptr);
    ~ElementVisualizer();

    // Update the visualized element (nucleus + electrons)
    void updateElement(const PlaygroundLib::Element& element);

private:
    Qt3DCore::QEntity* m_rootEntity;        // Parent entity
    Qt3DCore::QEntity* m_currentEntity;     // Main entity representing the atom
    Qt3DCore::QEntity* m_atomChildrenEntity; // Holds nucleus + electrons
    Qt3DCore::QTransform* m_transform;      // Transform for rotating the atom
    QVariantAnimation* m_rotationAnim;      // Animation for rotating the atom

    // Rotate the whole atom
    void setRotation(float angle);
};
