#include "periodictablewindow.h"
#include "ui_periodictablewindow.h"

#include <Mendelejew/Element.hpp>
#include <Mendelejew/ElementReader.hpp>
#include <QHelpEvent>
#include <QTableWidgetItem>
#include <QToolTip>
#include <vector>

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
  table->setItem(6-1, 3-1, lanthanides);

  QTableWidgetItem* actinides =
    new QTableWidgetItem(QString::fromStdString("Act\n (89-103)"));
  table->setItem(7-1, 3-1, actinides);

  table->setMouseTracking(true);
  // table->resizeColumnsToContents();
}

PeriodicTableWindow::PeriodicTableWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::PeriodicTableWindow)
{
  ui->setupUi(this);

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

      connect(ui->tablePeriodic, &QTableWidget::cellClicked,
            this, &PeriodicTableWindow::onTableCellClicked);
}

void PeriodicTableWindow::onTableCellClicked(int row, int column)
{
  QTableWidgetItem *item = ui->tablePeriodic->item(row, column);
    if (!item)
        return;

    // Example: get element info (you can adapt this to your actual data)
    QString elementSymbol = item->text();
    auto it = std::find_if(elements.begin(), elements.end(),
                           [&elementSymbol](const PlaygroundLib::Element& elem)
                           {
                               return QString::fromStdString(elem.getSymbol()) == elementSymbol;
                           });

    if(it == elements.end())
        return;

    QString infoText = QString();
    const auto& properties = it->getAllProperties();
    for (const auto& [key, value] : properties)
    {
        infoText += QString("<b>%1:</b> %2<br>").arg(QString::fromStdString(key), QString::fromStdString(value));
    }
    // Fill the period info label
    /*
    QString infoText = QString(
        "<b>Element:</b> %1<br>"
        "<b>Period:</b> %2<br>"
        "<b>Group:</b> %3<br>"
        "<b>Group:</b> %3<br>")
        .arg(it->getName())
        .arg(it->getPeriod())
        .arg(it->getGroup())
        .arg(it->);
        /
        */



    ui->labelPeriodInfoContent->setText(infoText);
}

PeriodicTableWindow::~PeriodicTableWindow()
{
  delete ui;
}
