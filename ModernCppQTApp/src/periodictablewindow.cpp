#include "periodictablewindow.h"
#include "ui_periodictablewindow.h"

// from static lib PlaygroundLib
#include <Mendelejew/Element.hpp>
#include <Mendelejew/ElementReader.hpp>

// from Qt
#include <QHelpEvent>
#include <QQmlContext>
#include <QQuickWidget>
#include <QTableWidgetItem>
#include <QTimer>
#include <QToolTip>
#include <QVariantAnimation>
#include <QVulkanInstance>
#include <QVulkanWindow>
#include <string>
#include <vector>

// Qt3D includes
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DRender/QDirectionalLight>

#include <QForwardRenderer>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraLens>

// Local vulkan window includes

#include <iostream>

#ifdef __APPLE__
#include "MacOS/MetalItem.h"
#endif

void fillPeriodicTable(QTableWidget* table,
                       const std::vector<PlaygroundLib::Element>& elements)
{
  for (const auto& elem : elements)
  {
    int row = 0;
    int column = 0;

    if (elem.getGroup() > 0)
    {
      // Normal element with defined group
      row = elem.getPeriod() - 1;   // 0-indexed
      column = elem.getGroup() - 1; // 0-indexed
    }
    else if (elem.getGroup() == -1)
    {
      // Special case: Lanthanides and Actinides (f-block)
      if (elem.getAtomicNumber() >= 57 && elem.getAtomicNumber() <= 71)
      {
        row = table->rowCount() - 2; // Lanthanides, second last row
        column = (elem.getAtomicNumber() - 57) + 2; // Place left to right
      }
      else if (elem.getAtomicNumber() >= 89 && elem.getAtomicNumber() <= 103)
      {
        row = table->rowCount() - 1;                // Actinides, last row
        column = (elem.getAtomicNumber() - 89) + 2; // Place left to right
      }
      else
      {
        // fallback for any other element without group
        row = elem.getPeriod() - 1;
        column = 0;
      }
    }

    QTableWidgetItem* item =
      new QTableWidgetItem(QString::fromStdString(elem.getSymbol()));
    item->setTextAlignment(Qt::AlignCenter);
    item->setToolTip(QString::fromStdString(
      "Element: " + elem.getName() + " (" + elem.getSymbol() + ")\n" +
      "Atomic Number: " + std::to_string(elem.getAtomicNumber()) + "\n" +
      "Atomic Weight: " + std::to_string(elem.getAtomicWeight()) + "\n" +
      "Protons: " + std::to_string(elem.getNumberOfProtons()) + "\n" +
      "Neutrons: " + std::to_string(elem.getNumberOfNeutrons()) + "\n" +
      "Electrons: " + std::to_string(elem.getNumberOfElectrons()) + "\n"));

    // set metals to light gray
    if (elem.getPhaseAtSTP() == "solid")
    {
      item->setBackground(QBrush(Qt::lightGray));
    }

    // set gases to light yellow
    if (elem.getPhaseAtSTP() == "gas")
    {
      item->setBackground(QBrush(QColor(255, 255, 150)));
      item->setForeground(QBrush(QColor(0, 0, 0)));
    }

    // set liquids to light cyan
    if (elem.getPhaseAtSTP() == "liq")
    {
      item->setBackground(QBrush(Qt::cyan));
    }

    // Highlight radioactive elements
    if (elem.isRadioactive())
    {
      item->setBackground(QBrush(Qt::green));
    }

    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    table->setItem(row, column, item);

    // Set atomic number as tooltip
  }

  // for 3-6 and 3-7 put information about Lanthanides and Actinides

  QTableWidgetItem* lanthanides =
    new QTableWidgetItem(QString::fromStdString("Lan\n (57-71)"));
  table->setItem(6 - 1, 3 - 1, lanthanides);

  QTableWidgetItem* actinides =
    new QTableWidgetItem(QString::fromStdString("Act\n (89-103)"));
  table->setItem(7 - 1, 3 - 1, actinides);

  table->setMouseTracking(true);
  // table->resizeColumnsToContents();
}

PeriodicTableWindow::PeriodicTableWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::PeriodicTableWindow)
{
  ui->setupUi(this);

#ifdef __APPLE__
  PrepereAppleScene(ui);
#endif

  // Read elements from
  std::filesystem::path exePath = std::filesystem::
    current_path(); // or use QCoreApplication::applicationDirPath() with Qt
  std::filesystem::path filePath = exePath / "data/Elements.csv";

  elements = PlaygroundLib::ElementReader::ReadElementsFromFile(filePath);

  fillPeriodicTable(ui->tablePeriodic, elements);

  // Optional: adjust table style
  ui->tablePeriodic->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->tablePeriodic->setSelectionMode(QAbstractItemView::NoSelection);

  int size = 40; // pixel size of each square
  ui->tablePeriodic->horizontalHeader()->setDefaultSectionSize(size);
  ui->tablePeriodic->verticalHeader()->setDefaultSectionSize(size);

  ui->tablePeriodic->viewport()->setAttribute(Qt::WA_AlwaysShowToolTips);
  ui->tablePeriodic->viewport()->installEventFilter(this);
  ui->tablePeriodic->viewport()->setMouseTracking(true);

  connect(ui->tablePeriodic,
          &QTableWidget::cellClicked,
          this,
          &PeriodicTableWindow::onTableCellClicked);
}

void PeriodicTableWindow::onTableCellClicked(int row, int column)
{
  QTableWidgetItem* item = ui->tablePeriodic->item(row, column);
  if (!item)
    return;

  // Example: get element info (you can adapt this to your actual data)
  QString elementSymbol = item->text();
  auto it = std::find_if(
    elements.begin(),
    elements.end(),
    [&elementSymbol](const PlaygroundLib::Element& elem)
    { return QString::fromStdString(elem.getSymbol()) == elementSymbol; });

  if (it == elements.end())
    return;

  QString infoText = QString();
  const auto& properties = it->getAllProperties();
  for (const auto& [key, value] : properties)
  {
    if (key != "Element" &&
        key != "Symbol") // Skip name and symbol as they are in title
      infoText +=
        QString("<b>%1:</b> %2<br>")
          .arg(QString::fromStdString(key), QString::fromStdString(value));
  }
  ui->labelPeriodInfoContent->setText(infoText);

  ui->labelPeriodInfoTitle->setText(
    QString("%1 (%2)").arg(QString::fromStdString(it->getName()),
                           QString::fromStdString(it->getSymbol())));

  QVariantAnimation* anim = new QVariantAnimation(this);
  anim->setDuration(600);
  anim->setStartValue(QColor(255, 255, 180));
  anim->setEndValue(QColorConstants::White);
  anim->setEasingCurve(QEasingCurve::OutCubic);

  connect(anim,
          &QVariantAnimation::valueChanged,
          this,
          [item](const QVariant& value)
          { item->setBackground(value.value<QColor>()); });

  bool isRadioactive = it->isRadioactive();
  std::string phase = it->getPhaseAtSTP();
  connect(anim,
          &QVariantAnimation::finished,
          this,
          [item, isRadioactive, phase]()
          {
            if (isRadioactive)
            {
              item->setBackground(QBrush(Qt::green));
              return;
            }
            else
            {
              item->setBackground(Qt::NoBrush);
            }

            if (phase == "solid")
            {
              item->setBackground(QBrush(Qt::lightGray));
            }
            else if (phase == "gas")
            {
              item->setBackground(QBrush(QColor(255, 255, 150)));
              item->setForeground(QBrush(QColor(0, 0, 0)));
            }
            else if (phase == "liq")
            {
              item->setBackground(QBrush(Qt::cyan));
            }
            else
            {
              item->setBackground(Qt::NoBrush);
            }
          });

  anim->start(QAbstractAnimation::DeleteWhenStopped);
}

PeriodicTableWindow::~PeriodicTableWindow()
{
  delete ui;
}

void PeriodicTableWindow::PrepereAppleScene(Ui::PeriodicTableWindow* ui)
{
  // Create Qt3D window
  auto* view3D = new Qt3DExtras::Qt3DWindow();
  Qt3DExtras::QForwardRenderer* forwardRenderer =
    static_cast<Qt3DExtras::QForwardRenderer*>(view3D->defaultFrameGraph());
  if (forwardRenderer)
    forwardRenderer->setClearColor(QColor(Qt::black));

  QWidget* container = QWidget::createWindowContainer(view3D);
  container->setMinimumSize(300, 200);
  container->setMaximumHeight(250);
  container->setFocusPolicy(Qt::StrongFocus);

  // Replace placeholder inside verticalLayoutPeriodInfo
  QVBoxLayout* infoLayout = ui->verticalLayoutPeriodInfo;
  int index = infoLayout->indexOf(ui->metalWidgetPlaceholder);
  if (index >= 0)
  {
    infoLayout->removeWidget(ui->metalWidgetPlaceholder);
    ui->metalWidgetPlaceholder->deleteLater();
    infoLayout->insertWidget(index, container);
  }

  // ---- Create scene ----
  Qt3DCore::QEntity* rootEntity = new Qt3DCore::QEntity();

  Qt3DCore::QEntity* cubeEntity = new Qt3DCore::QEntity(rootEntity);
  auto* cubeMesh = new Qt3DExtras::QCuboidMesh();
  cubeEntity->addComponent(cubeMesh);

  auto* cubeMaterial = new Qt3DExtras::QPhongMaterial();
  cubeMaterial->setDiffuse(QColor(Qt::red));
  cubeEntity->addComponent(cubeMaterial);

  auto* cubeTransform = new Qt3DCore::QTransform();
  cubeEntity->addComponent(cubeTransform);

  // Animate rotation
  auto* animation = new QVariantAnimation(this);
  animation->setStartValue(0.0f);
  animation->setEndValue(360.0f);
  animation->setDuration(4000);
  animation->setLoopCount(-1);
  connect(animation,
          &QVariantAnimation::valueChanged,
          [cubeTransform](const QVariant& v)
          {
            cubeTransform->setRotation(
              QQuaternion::fromEulerAngles(0, v.toFloat(), 0));
          });
  animation->start();

  Qt3DCore::QEntity* dirLightEntity = new Qt3DCore::QEntity(rootEntity);
  auto* dirLight = new Qt3DRender::QDirectionalLight(dirLightEntity);
  dirLight->setWorldDirection(QVector3D(-1.0f, -1.0f, -1.0f));
  dirLight->setColor(Qt::white);
  dirLight->setIntensity(1.0f); // brightness
  dirLightEntity->addComponent(dirLight);

  // Camera
  Qt3DRender::QCamera* camera = view3D->camera();
  camera->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
  camera->setPosition(QVector3D(0, 0, 5.0f));
  camera->setViewCenter(QVector3D(0, 0, 0));

  auto* camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
  camController->setCamera(camera);

  view3D->setRootEntity(rootEntity);
}