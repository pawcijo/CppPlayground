#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Load demo map
    auto rawMap = DemoFactory::getDemoMap();
    int id = 1;
    for (const auto& [demoType, pair] : rawMap) {
        demoMap[id] = { demoType, pair.second };
        ui->demoSelector->addItem(QString::fromStdString(pair.second), id);
        ++id;
    }

    connect(ui->runButton, &QPushButton::clicked, this, &MainWindow::onRunClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onRunClicked() {
    int index = ui->demoSelector->currentIndex();
    int selectedId = ui->demoSelector->itemData(index).toInt();

    if (!demoMap.contains(selectedId)) {
        QMessageBox::warning(this, "Error", "Invalid demo selected.");
        return;
    }

    auto [type, name] = demoMap[selectedId];

    demoInstance = DemoFactory::createDemo(type);
    if (demoInstance) {
        demoInstance->ShowDemo();
    } else {
        QMessageBox::information(this, "Not Implemented", "This demo is not implemented yet.");
    }
}
