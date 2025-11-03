#pragma once
#include <QMainWindow>

namespace Ui
{
class PeriodicTableWindow; // forward declaration from ui_PeriodicTable.h
}

class PeriodicTableWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit PeriodicTableWindow(QWidget* parent = nullptr);
  ~PeriodicTableWindow();

private:
  Ui::PeriodicTableWindow* ui;

};
