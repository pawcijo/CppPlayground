#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"
#include <vector>

class OutOfRangeDemo : public DemoBase
{
public:
  OutOfRangeDemo()
  {
    mName = "OutOfRangeDemo";
    mNotes = { { "Notes for this demo are in terminal.", "" } };
  };
  void ShowExample(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    LOG_START_FUNCTION();
    if (nullptr == printNotesCallback)
    {
      PrintNotes();
    }
    else
    {
      printNotesCallback(mNotes);
    }

    LOG_END_FUNCTION();
  }
  void ShowDemo(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowExample(printNotesCallback);

    std::vector<int> vec = { 1, 2, 3, 4, 5 };

    try
    {
      // This will throw an exception if the index is out of range
      std::cout << "Accessing element at index 10: " << vec.at(10) << std::endl;
    }
    catch (const std::out_of_range& e)
    {
      std::cout << RED_BOLD_TEXT_START
                << "Caught an out_of_range exception: " << e.what()
                << TEXT_COLOR_CLEAR << std::endl;
    }
  }
};