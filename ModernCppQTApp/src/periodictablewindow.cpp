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

// Local vulkan window includes
#include "MyVulkanItem.h"

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

  qmlRegisterType<MyVulkanItem>("MyVulkan", 1, 0, "VulkanItem");

  auto* quickWidget = new QQuickWidget(this);
  quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

  // Load QML from resource or local file
  quickWidget->setSource(QUrl(QStringLiteral("qrc:/VulkanPanel.qml")));
  quickWidget->setMinimumSize(300, 200);

  // Replace placeholder
  ui->verticalLayoutPeriodInfo->replaceWidget(ui->vulkanWidgetPlaceholder,
                                              quickWidget);
  ui->vulkanWidgetPlaceholder->deleteLater();

  m_vkInstance.setExtensions({ "VK_KHR_surface", "VK_KHR_portability_subset" });
  if (!m_vkInstance.create())
    qWarning() << "Vulkan instance creation FAILED!";

  // Pass instance to the QQuickItem after loading QML
  QObject* rootObj = quickWidget->rootObject();
  if (auto* vulkanItem = rootObj->findChild<MyVulkanItem*>("vulkanItem"))
  {
    vulkanItem->setVulkanInstance(&m_vkInstance);
  }

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
