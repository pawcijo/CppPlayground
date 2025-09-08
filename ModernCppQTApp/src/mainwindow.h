#pragma once

//incliude from Qt Core
#include <QMainWindow>
#include <memory>
#include <map>

#include "DemosFactory/DemoFactory.hpp" // Your existing factory with DemoType, getDemoMap, createDemo

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onRunClicked();
    void onClearClicked();
    void onTagFilterChanged(); // Add this line

private:
    void writeNotesToTerminal(NoteFormat& notes);
    void updateDemoSelector(); // Add this line

    std::unique_ptr<DemoBase> createDemo(DemoType chosenDemo);
    Ui::MainWindow *ui;
    std::unordered_map<DemoType, DemoFactory::DemoCreatorWithNameAndTags> rawMap;
    std::unique_ptr<DemoBase> demoInstance;
    std::map<int, std::pair<DemoType, std::string>> filteredDemoMap; // Add this line
};
