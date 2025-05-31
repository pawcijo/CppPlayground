#include "mainwindow.h"
#include "ui_mainwindow.h"

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
  auto rawMap = DemoFactory::getDemoMap();
  int id = 1;
  for (const auto& [demoType, pair] : rawMap)
  {
    demoMap[id] = { demoType, std::get<1>(pair) };
    ++id;
  }

  // Connect tag checkboxes
  connect(ui->tagCommon, &QCheckBox::checkStateChanged, this, &MainWindow::onTagFilterChanged);
  connect(ui->tagDesignPatterns, &QCheckBox::checkStateChanged, this, &MainWindow::onTagFilterChanged);
  connect(ui->tagCPP17, &QCheckBox::checkStateChanged, this, &MainWindow::onTagFilterChanged);
  connect(ui->tagCPP20, &QCheckBox::checkStateChanged, this, &MainWindow::onTagFilterChanged);
  connect(ui->tagNeuralNetwork, &QCheckBox::checkStateChanged, this, &MainWindow::onTagFilterChanged);

  connect(
    ui->runButton, &QPushButton::clicked, this, &MainWindow::onRunClicked);
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
  if (ui->tagCommon->isChecked()) selectedTags.push_back(DemoTag::Common);
  if (ui->tagDesignPatterns->isChecked()) selectedTags.push_back(DemoTag::DesignPatterns);
  if (ui->tagCPP17->isChecked()) selectedTags.push_back(DemoTag::CPP17);
  if (ui->tagCPP20->isChecked()) selectedTags.push_back(DemoTag::CPP20);
  if (ui->tagNeuralNetwork->isChecked()) selectedTags.push_back(DemoTag::NeuralNetwork);

  // Clear selector and filtered map
  ui->demoSelector->clear();
  filteredDemoMap.clear();

  int id = 1;
  for (const auto& [demoType, tuple] : DemoFactory::getDemoMap())
  {
    const auto& tags = std::get<2>(tuple);
    bool matches = false;
    for (const auto& tag : tags)
    {
      if (std::find(selectedTags.begin(), selectedTags.end(), tag) != selectedTags.end())
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

  demoInstance = DemoFactory::createDemo(type);
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
    cursor.insertText(QString::fromStdString(pair.second + "\n\n"), descFormat);
  }
  ui->terminalOutput->moveCursor(QTextCursor::Start);
}

void MainWindow::onClearClicked()
{
  ui->terminalOutput->clear();
}
