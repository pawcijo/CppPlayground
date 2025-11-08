#pragma once
#include "MacOS/ElementVisualizer.h"
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

  Ui::PeriodicTableWindow* ui = nullptr;
  Qt3DCore::QEntity* rootEntity = nullptr; 
  ElementVisualizer* m_elementVisualizer = nullptr;
  QVulkanInstance m_vkInstance;
  std::vector<PlaygroundLib::Element> elements;
};
