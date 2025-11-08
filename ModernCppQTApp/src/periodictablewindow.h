#pragma once
#include <Mendelejew/Element.hpp>
#include <QMainWindow>
#include <QVulkanInstance>

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
#ifdef __APPLE__
  void PrepereAppleScene(Ui::PeriodicTableWindow* ui);
#endif

  Ui::PeriodicTableWindow* ui;
  QVulkanInstance m_vkInstance;
  std::vector<PlaygroundLib::Element> elements;
};
