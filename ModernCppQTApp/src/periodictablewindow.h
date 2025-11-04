#pragma once
#include <QMainWindow>
#include <Mendelejew/Element.hpp>

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

  private slots:
    void onTableCellClicked(int row, int column);

private:
  Ui::PeriodicTableWindow* ui;
  std::vector<PlaygroundLib::Element> elements;

};
