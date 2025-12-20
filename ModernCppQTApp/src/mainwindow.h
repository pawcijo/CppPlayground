#pragma once

// incliude from Qt Core
#include <QMainWindow>
#include <map>
#include <memory>

// typer
#include <QTextCursor>
#include <QTimer>
#include <deque>

#include "DemosFactory/DemoFactory.hpp" // Your existing factory with DemoType, getDemoMap, createDemo
#include "periodictablewindow.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

struct TypedChar
{
  QChar ch;
  QTextCharFormat format;
};

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

private slots:
  void onRunClicked();
  void onMendelejewClicked();
  void onClearClicked();
  void onTagFilterChanged(); // Add this line
  void writeNotesToTerminal(const NoteFormat& notes);
  void writeNotesToTerminalFast(const NoteFormat& notes);
  void writeNotesToTerminalTyping(const NoteFormat& notes);

  void typeNextChar();

private:
  void updateDemoSelector(); // Add this line

  QTimer* typingTimer = nullptr;
  std::deque<TypedChar> typingQueue;
  QTextCursor typingCursor;
  std::unique_ptr<DemoBase> createDemo(DemoType chosenDemo);
  Ui::MainWindow* ui;
  PeriodicTableWindow* periodicTableWindow = nullptr;
  std::unordered_map<DemoType, DemoFactory::DemoCreatorWithNameAndTags> rawMap;
  std::unique_ptr<DemoBase> demoInstance;
  std::map<int, std::pair<DemoType, std::string>>
    filteredDemoMap; // Add this line
};
