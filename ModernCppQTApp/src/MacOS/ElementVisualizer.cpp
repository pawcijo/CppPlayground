#include "ElementVisualizer.h"
#include <QDebug>
#include <QQuaternion>
#include <QRandomGenerator>
#include <QVariantAnimation>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QSphereMesh>
#include <QtMath>

#include <Mendelejew/Element.hpp>

static QVector3D randomPositionInSphere(float radius)
{
  float u = QRandomGenerator::global()->generateDouble();
  float v = QRandomGenerator::global()->generateDouble();
  float theta = u * 2.0 * M_PI;
  float phi = acos(2.0 * v - 1.0);
  float r = radius * cbrt(QRandomGenerator::global()->generateDouble());
  float x = r * sin(phi) * cos(theta);
  float y = r * sin(phi) * sin(theta);
  float z = r * cos(phi);
  return QVector3D(x, y, z);
}

ElementVisualizer::ElementVisualizer(Qt3DCore::QEntity* rootEntity,
                                     QObject* parent)
  : QObject(parent)
  , m_rootEntity(rootEntity)
{

  qDebug() << "ElementVisualizer constructor called";
  // Main atom entity
  m_currentEntity = new Qt3DCore::QEntity(m_rootEntity);

  // Transform for rotating the entire atom
  m_transform = new Qt3DCore::QTransform();
  m_currentEntity->addComponent(m_transform);

  // Container for nucleus + electrons
  m_atomChildrenEntity = new Qt3DCore::QEntity(m_currentEntity);

  // Rotation animation
  m_rotationAnim = new QVariantAnimation(this);
  m_rotationAnim->setDuration(8000);
  m_rotationAnim->setStartValue(0.0f);
  m_rotationAnim->setEndValue(360.0f);
  m_rotationAnim->setEasingCurve(QEasingCurve::Linear);
  m_rotationAnim->setLoopCount(-1);
  connect(m_rotationAnim,
          &QVariantAnimation::valueChanged,
          this,
          [this](const QVariant& value) { setRotation(value.toFloat()); });
  m_rotationAnim->start();
}

ElementVisualizer::~ElementVisualizer()
{

  qDebug() << "ElementVisualizer destructor called";
  if (m_rotationAnim)
  {
    m_rotationAnim->stop();
    delete m_rotationAnim;
    m_rotationAnim = nullptr;
  }

  // Delete atom entity (owns transform and children)
  if (m_currentEntity)
  {
    delete m_currentEntity;
    m_currentEntity = nullptr;
  }
}

void ElementVisualizer::updateElement(const PlaygroundLib::Element& element)
{
    // Clear previous nucleus/electrons
    const auto& children = m_atomChildrenEntity->children();
    for (QObject* child : children)
        delete child;

    // ---------------- NUCLEUS ----------------
    Qt3DCore::QEntity* nucleus = new Qt3DCore::QEntity(m_atomChildrenEntity);

    for (int i = 0; i < element.getNumberOfProtons(); ++i)
    {
        auto* proton = new Qt3DCore::QEntity(nucleus);
        auto* mesh = new Qt3DExtras::QSphereMesh();
        mesh->setRadius(0.1f);
        auto* material = new Qt3DExtras::QPhongMaterial();
        material->setDiffuse(QColor(255, 0, 0));
        proton->addComponent(mesh);
        proton->addComponent(material);

        auto* transform = new Qt3DCore::QTransform();
        transform->setTranslation(randomPositionInSphere(0.2f));
        proton->addComponent(transform);
    }

    for (int i = 0; i < element.getNumberOfNeutrons(); ++i)
    {
        auto* neutron = new Qt3DCore::QEntity(nucleus);
        auto* mesh = new Qt3DExtras::QSphereMesh();
        mesh->setRadius(0.1f);
        auto* material = new Qt3DExtras::QPhongMaterial();
        material->setDiffuse(QColor(0, 0, 255));
        neutron->addComponent(mesh);
        neutron->addComponent(material);

        auto* transform = new Qt3DCore::QTransform();
        transform->setTranslation(randomPositionInSphere(0.2f));
        neutron->addComponent(transform);
    }

    // ---------------- ELECTRONS ----------------
    int totalElectrons = element.getNumberOfElectrons();

    // Orbital filling order: s, p, d, f (simplified)
    QVector<int> orbitalCapacities = {2, 2, 6, 2, 6, 10, 2, 6, 10, 14, 2, 6, 10, 14};
    QVector<float> shellRadius = {0.5f, 0.8f, 1.1f, 1.4f, 1.7f, 2.0f, 2.3f, 2.6f};

    int electronIndex = 0;
    int shell = 0;

    while (electronIndex < totalElectrons)
    {
        // Safe capacity: fallback to 2 if shell index exceeds array
        int capacity = (shell < orbitalCapacities.size()) ? orbitalCapacities[shell] : 2;
        int electronsInThisShell = qMin(capacity, totalElectrons - electronIndex);

        float radius = (shell < shellRadius.size()) ? shellRadius[shell] : 0.5f + shell * 0.3f;

        for (int e = 0; e < electronsInThisShell; ++e)
        {
            auto* electron = new Qt3DCore::QEntity(m_atomChildrenEntity);
            auto* mesh = new Qt3DExtras::QSphereMesh();
            mesh->setRadius(0.05f);
            auto* material = new Qt3DExtras::QPhongMaterial();
            material->setDiffuse(QColor(255, 255, 0));
            material->setSpecular(QColor(255, 255, 255));
            material->setShininess(30.0f);
            electron->addComponent(mesh);
            electron->addComponent(material);

            auto* transform = new Qt3DCore::QTransform();
            electron->addComponent(transform);

            QVariantAnimation* anim = new QVariantAnimation(electron);
            anim->setDuration(4000 + shell * 1000);
            anim->setStartValue(0.0f);
            anim->setEndValue(360.0f);
            anim->setLoopCount(-1);
            QObject::connect(anim, &QVariantAnimation::valueChanged,
                             [transform, radius, e, electronsInThisShell](const QVariant& val)
                             {
                                 float angle = val.toFloat() + (360.0f / electronsInThisShell) * e;
                                 float rad = qDegreesToRadians(angle);
                                 transform->setTranslation(QVector3D(radius*qCos(rad), 0, radius*qSin(rad)));
                             });
            anim->start();
        }

        electronIndex += electronsInThisShell;
        shell++;
    }
}


void ElementVisualizer::setRotation(float angle)
{
  if (m_transform)
    m_transform->setRotation(QQuaternion::fromEulerAngles(0.0f, angle, 0.0f));
}
