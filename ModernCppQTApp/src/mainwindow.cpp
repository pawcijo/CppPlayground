#include "mainwindow.h"
#include "periodictablewindow.h"
#include "ui_mainwindow.h"
#include "ui_periodictablewindow.h"

#include <QCheckBox>
#include <QGroupBox>
#include <QMessageBox>
#include <QString>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextEdit>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  // Load demo map
  rawMap = DemoFactory::getDemoMap();

  // Connect tag checkboxes
  connect(ui->tagCommon,
          &QCheckBox::checkStateChanged,
          this,
          &MainWindow::onTagFilterChanged);
  connect(ui->tagDesignPatterns,
          &QCheckBox::checkStateChanged,
          this,
          &MainWindow::onTagFilterChanged);
  connect(ui->tagCPP17,
          &QCheckBox::checkStateChanged,
          this,
          &MainWindow::onTagFilterChanged);
  connect(ui->tagCPP20,
          &QCheckBox::checkStateChanged,
          this,
          &MainWindow::onTagFilterChanged);
  connect(ui->tagGreboszCPP,
          &QCheckBox::checkStateChanged,
          this,
          &MainWindow::onTagFilterChanged);
  connect(ui->tagNeuralNetwork,
          &QCheckBox::checkStateChanged,
          this,
          &MainWindow::onTagFilterChanged);
  connect(ui->tagQuestionTask,
          &QCheckBox::checkStateChanged,
          this,
          &MainWindow::onTagFilterChanged);

  connect(
    ui->runButton, &QPushButton::clicked, this, &MainWindow::onRunClicked);
  connect(ui->openTableButton,
          &QPushButton::clicked,
          this,
          &MainWindow::onMendelejewClicked);
  connect(
    ui->clearButton, &QPushButton::clicked, this, &MainWindow::onClearClicked);

  updateDemoSelector();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::onTagFilterChanged()
{
  updateDemoSelector();
}

void MainWindow::updateDemoSelector()
{
  // Get selected tags
  std::vector<DemoTag> selectedTags;
  if (ui->tagCommon->isChecked())
    selectedTags.push_back(DemoTag::Common);
  if (ui->tagDesignPatterns->isChecked())
    selectedTags.push_back(DemoTag::DesignPatterns);
  if (ui->tagCPP17->isChecked())
    selectedTags.push_back(DemoTag::CPP17);
  if (ui->tagCPP20->isChecked())
    selectedTags.push_back(DemoTag::CPP20);
  if (ui->tagGreboszCPP->isChecked())
    selectedTags.push_back(DemoTag::GreboszCPP);
  if (ui->tagNeuralNetwork->isChecked())
    selectedTags.push_back(DemoTag::NeuralNetwork);
  if (ui->tagQuestionTask->isChecked())
    selectedTags.push_back(DemoTag::QuestionTask);

  // Clear selector and filtered map
  ui->demoSelector->clear();
  filteredDemoMap.clear();

  int id = 1;
  for (const auto& [demoType, tuple] : rawMap)
  {
    const auto& tags = std::get<2>(tuple);
    bool matches = false;
    for (const auto& tag : tags)
    {
      if (std::find(selectedTags.begin(), selectedTags.end(), tag) !=
          selectedTags.end())
      {
        matches = true;
        break;
      }
    }
    if (matches)
    {
      filteredDemoMap[id] = { demoType, std::get<1>(tuple) };
      ui->demoSelector->addItem(QString::fromStdString(std::get<1>(tuple)), id);
      ++id;
    }
  }
}

std::unique_ptr<DemoBase> MainWindow::createDemo(DemoType chosenDemo)
{
  const auto& it = rawMap.find(chosenDemo);
  if (it != rawMap.end())
  {
    std::cout << "Creating demo: " << "\n";
    return std::get<0>(it->second)(); // Call the associated lambda function
                                      // to create the object
  }
  return nullptr;
}

void MainWindow::onRunClicked()
{
  int index = ui->demoSelector->currentIndex();
  int selectedId = ui->demoSelector->itemData(index).toInt();

  if (!filteredDemoMap.contains(selectedId))
  {
    QMessageBox::warning(this, "Error", "Invalid demo selected.");
    return;
  }

  auto [type, name] = filteredDemoMap[selectedId];

  // clear previous object
  if (demoInstance)
  {
    demoInstance.reset();
  }

  demoInstance = createDemo(type);
  if (demoInstance)
  {
    // Pass the callback
    demoInstance->ShowDemo([this](NoteFormat& notes)
                           { writeNotesToTerminal(notes); });
  }
  else
  {
    QMessageBox::information(
      this, "Not Implemented", "This demo is not implemented yet.");
  }
}

void MainWindow::onMendelejewClicked()
{

    if (!periodicTableWindow) {
        periodicTableWindow = new PeriodicTableWindow(this);
        periodicTableWindow->setAttribute(Qt::WA_DeleteOnClose);

        // Clear pointer when window is closed
        connect(periodicTableWindow, &QWidget::destroyed, this, [this](){
            periodicTableWindow = nullptr;
        });
    }

    periodicTableWindow->show();
    periodicTableWindow->raise();
    periodicTableWindow->activateWindow();
}

void MainWindow::writeNotesToTerminal(NoteFormat& notes)
{
  ui->terminalOutput->clear();
  QTextCursor cursor(ui->terminalOutput->document());

  // Set desired font size
  int fontSize = 14; // Increase this value as needed

  for (const auto& pair : notes)
  {
    // Bold for title
    QTextCharFormat boldFormat;
    boldFormat.setFontWeight(QFont::Bold);
    boldFormat.setFontPointSize(fontSize);
    cursor.insertText(QString::fromStdString(pair.first + "\n"), boldFormat);

    // Color for description
    QTextCharFormat descFormat;
    descFormat.setForeground(QColor("#007acc")); // Example: blue
    descFormat.setFontPointSize(fontSize);
    cursor.insertText(QString::fromStdString(pair.second + "\n"), descFormat);
  }
  ui->terminalOutput->moveCursor(QTextCursor::Start);
}

void MainWindow::onClearClicked()
{
  ui->terminalOutput->clear();
}
