#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QString>
#include <QTextEdit>
#include <QTextCursor>
#include <QTextCharFormat>

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
        // Pass the callback
        demoInstance->ShowDemo([this](NoteFormat& notes) { writeNotesToTerminal(notes); });
    } else {
        QMessageBox::information(this, "Not Implemented", "This demo is not implemented yet.");
    }
}

void MainWindow::writeNotesToTerminal(NoteFormat& notes) {

    std::cout<< "Writing notes to terminal output." << std::endl;
    ui->terminalOutput->clear();
    QTextCursor cursor(ui->terminalOutput->document());
    for (const auto& pair : notes) {
        // Bold for title
        QTextCharFormat boldFormat;
        boldFormat.setFontWeight(QFont::Bold);
        cursor.insertText(QString::fromStdString(pair.first + "\n"), boldFormat);

        // Color for description
        QTextCharFormat descFormat;
        descFormat.setForeground(QColor("#007acc")); // Example: blue
        cursor.insertText(QString::fromStdString(pair.second + "\n\n"), descFormat);
    }
    ui->terminalOutput->moveCursor(QTextCursor::Start);
}
