#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include <ranges>

class ViewDemo : public DemoBase
{

public:
  ViewDemo()
  {

    mName = "ViewDemo";
    mNotes = {
      { "C++20 Views",
        "C++20 introduced ranges and views, which provide a powerful way to "
        "work with sequences of data in a lazy and composable manner. "
        "Views allow you to create lightweight, non-owning "
        "projections of data that can be transformed and filtered on-the-fly." }

    };
  }

  void ShowDemo(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {

    LOG_START_FUNCTION_CLASS();
    if (nullptr == printNotesCallback)
    {
      PrintNotes();
    }
    else
    {
      printNotesCallback(mNotes);
    }

    std::vector<int> numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    // Create a view that filters even numbers and then transforms them by
    // squaring
    auto evenSquaresView =
      numbers | std::views::filter([](int n) { return n % 2 == 0; }) |
      std::views::transform([](int n) { return n * n; });

    std::cout << "Even squares: ";
    for (int n : evenSquaresView)
    {
      std::cout << n << " ";
    }
    std::cout << std::endl;
    std::cout << "evenSquaresView type: " << typeid(evenSquaresView).name() << std::endl;
    std::cout << "evenSquaresView element type:" << typeid(decltype(*evenSquaresView.begin())).name() << std::endl;

    std::cout << std::endl;
  }

  void ShowExample(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowDemo(printNotesCallback);
  }
};