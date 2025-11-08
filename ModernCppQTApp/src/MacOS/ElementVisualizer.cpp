#include "ElementVisualizer.h"

#include <QDebug>
#include <QQuaternion>
#include <QRandomGenerator>
#include <QVariantAnimation>
#include <Qt3DCore/QAttribute>
#include <Qt3DCore/QBuffer>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QPerVertexColorMaterial>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DExtras/QCylinderMesh>
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

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QGeometry>
#include <Qt3DCore/QBuffer>
#include <Qt3DCore/QAttribute>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DExtras/QPhongMaterial>
#include <QVector3D>
#include <QColor>
#include <cmath>

void ElementVisualizer::createElectronOrbitTube(Qt3DCore::QEntity* parent,
                                                 float orbitRadius,
                                                 float tubeRadius ,
                                                 int segments,
                                                 int tubeSides,
                                                 const QColor& color)
{
    // Vertices and normals
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QVector<unsigned int> indices;

    for (int i = 0; i < segments; ++i)
    {
        float angle = 2.0f * M_PI * i / segments;
        QVector3D center(orbitRadius * cos(angle), 0.0f, orbitRadius * sin(angle));

        // Create small circle around center
        for (int j = 0; j < tubeSides; ++j)
        {
            float theta = 2.0f * M_PI * j / tubeSides;
            QVector3D offset(tubeRadius * cos(theta), tubeRadius * sin(theta), 0.0f);

            // Rotate offset around Y to match orbit direction
            QMatrix4x4 rot;
            rot.rotate(angle * 180.0f / M_PI, 0, 1, 0);
            QVector3D pos = center + rot.map(offset) ;

            vertices.append(pos);
            normals.append((rot.map(offset).normalized()));
        }
    }

    // Indices for triangles
    for (int i = 0; i < segments; ++i)
    {
        int nextSeg = (i + 1) % segments;
        for (int j = 0; j < tubeSides; ++j)
        {
            int nextSide = (j + 1) % tubeSides;

            int a = i * tubeSides + j;
            int b = nextSeg * tubeSides + j;
            int c = nextSeg * tubeSides + nextSide;
            int d = i * tubeSides + nextSide;

            // Two triangles per quad
            indices.append(a); indices.append(b); indices.append(c);
            indices.append(a); indices.append(c); indices.append(d);
        }
    }

    // Convert to QByteArray
    QByteArray vertexBytes(reinterpret_cast<const char*>(vertices.data()), vertices.size() * sizeof(QVector3D));
    QByteArray normalBytes(reinterpret_cast<const char*>(normals.data()), normals.size() * sizeof(QVector3D));
    QByteArray indexBytes(reinterpret_cast<const char*>(indices.data()), indices.size() * sizeof(unsigned int));

    // Buffers
    Qt3DCore::QBuffer* vertexBuffer = new Qt3DCore::QBuffer(parent);
    vertexBuffer->setData(vertexBytes);

    Qt3DCore::QBuffer* normalBuffer = new Qt3DCore::QBuffer(parent);
    normalBuffer->setData(normalBytes);

    Qt3DCore::QBuffer* indexBuffer = new Qt3DCore::QBuffer(parent);
    indexBuffer->setData(indexBytes);

    // Attributes
    Qt3DCore::QAttribute* posAttr = new Qt3DCore::QAttribute();
    posAttr->setName(Qt3DCore::QAttribute::defaultPositionAttributeName());
    posAttr->setVertexBaseType(Qt3DCore::QAttribute::Float);
    posAttr->setVertexSize(3);
    posAttr->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    posAttr->setBuffer(vertexBuffer);
    posAttr->setCount(vertices.size());

    Qt3DCore::QAttribute* normalAttr = new Qt3DCore::QAttribute();
    normalAttr->setName(Qt3DCore::QAttribute::defaultNormalAttributeName());
    normalAttr->setVertexBaseType(Qt3DCore::QAttribute::Float);
    normalAttr->setVertexSize(3);
    normalAttr->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    normalAttr->setBuffer(normalBuffer);
    normalAttr->setCount(normals.size());

    Qt3DCore::QAttribute* indexAttr = new Qt3DCore::QAttribute();
    indexAttr->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);
    indexAttr->setVertexBaseType(Qt3DCore::QAttribute::UnsignedInt);
    indexAttr->setBuffer(indexBuffer);
    indexAttr->setCount(indices.size());

    // Geometry
    Qt3DCore::QGeometry* geometry = new Qt3DCore::QGeometry(parent);
    geometry->addAttribute(posAttr);
    geometry->addAttribute(normalAttr);
    geometry->addAttribute(indexAttr);

    // Renderer
    Qt3DRender::QGeometryRenderer* renderer = new Qt3DRender::QGeometryRenderer(parent);
    renderer->setGeometry(geometry);
    renderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Triangles);

    // Material
    Qt3DExtras::QPhongMaterial* material = new Qt3DExtras::QPhongMaterial(parent);
    material->setDiffuse(color);

    // Entity
    Qt3DCore::QEntity* orbitEntity = new Qt3DCore::QEntity(parent);
    orbitEntity->addComponent(renderer);
    orbitEntity->addComponent(material);
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
    qDeleteAll(m_atomChildrenEntity->children());

    // ---------------- NUCLEUS ----------------
    Qt3DCore::QEntity* nucleus = new Qt3DCore::QEntity(m_atomChildrenEntity);

    auto* protonMesh = new Qt3DExtras::QSphereMesh();
    protonMesh->setRadius(0.1f);
    auto* protonMaterial = new Qt3DExtras::QPhongMaterial();
    protonMaterial->setDiffuse(QColor(255, 0, 0));

    for (int i = 0; i < element.getNumberOfProtons(); ++i)
    {
        auto* proton = new Qt3DCore::QEntity(nucleus);
        proton->addComponent(protonMesh);
        proton->addComponent(protonMaterial);

        auto* transform = new Qt3DCore::QTransform();
        transform->setTranslation(randomPositionInSphere(0.2f));
        proton->addComponent(transform);
    }

    auto* neutronMesh = new Qt3DExtras::QSphereMesh();
    neutronMesh->setRadius(0.1f);
    auto* neutronMaterial = new Qt3DExtras::QPhongMaterial();
    neutronMaterial->setDiffuse(QColor(0, 0, 255));

    for (int i = 0; i < element.getNumberOfNeutrons(); ++i)
    {
        auto* neutron = new Qt3DCore::QEntity(nucleus);
        neutron->addComponent(neutronMesh);
        neutron->addComponent(neutronMaterial);

        auto* transform = new Qt3DCore::QTransform();
        transform->setTranslation(randomPositionInSphere(0.2f));
        neutron->addComponent(transform);
    }

    // ---------------- ELECTRONS ----------------
    int totalElectrons = element.getNumberOfElectrons();
    QVector<int> orbitalCapacities = {2,2,6,2,6,10,2,6,10,14,2,6,10,14};
    QVector<float> shellRadius = {0.5f,0.8f,1.1f,1.4f,1.7f,2.0f,2.3f,2.6f};

    int electronIndex = 0;
    int shell = 0;

    auto* electronMesh = new Qt3DExtras::QSphereMesh();
    electronMesh->setRadius(0.05f);
    auto* electronMaterial = new Qt3DExtras::QPhongMaterial();
    electronMaterial->setDiffuse(QColor(255, 255, 0));

    while (electronIndex < totalElectrons)
    {
        int capacity = (shell < orbitalCapacities.size()) ? orbitalCapacities[shell] : 2;
        int electronsInShell = qMin(capacity, totalElectrons - electronIndex);
        float radius = (shell < shellRadius.size()) ? shellRadius[shell] : 0.5f + shell*0.3f;

        // --- Create orbit as a single line (pipe) ---
        createElectronOrbitTube(m_atomChildrenEntity, radius);

        for (int e = 0; e < electronsInShell; ++e)
        {
            auto* electron = new Qt3DCore::QEntity(m_atomChildrenEntity);
            electron->addComponent(electronMesh);
            electron->addComponent(electronMaterial);

            auto* transform = new Qt3DCore::QTransform();
            electron->addComponent(transform);

            QVariantAnimation* anim = new QVariantAnimation(electron);
            anim->setDuration(4000 + shell*1000);
            anim->setStartValue(0.0f);
            anim->setEndValue(360.0f);
            anim->setLoopCount(-1);
            QObject::connect(anim, &QVariantAnimation::valueChanged, [transform, radius, e, electronsInShell](const QVariant& val){
                float angle = val.toFloat() + (360.0f / electronsInShell) * e;
                float rad = qDegreesToRadians(angle);
                transform->setTranslation(QVector3D(radius*qCos(rad), 0, radius*qSin(rad)));
            });
            anim->start();
        }

        electronIndex += electronsInShell;
        shell++;
    }
}

void ElementVisualizer::setRotation(float angle)
{
  if (m_transform)
    m_transform->setRotation(QQuaternion::fromEulerAngles(0.0f, angle, 0.0f));
}
