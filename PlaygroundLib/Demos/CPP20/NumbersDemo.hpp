#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include <cmath>
#include <numbers>
#include <numeric>

class NumbersDemo : public DemoBase
{
public:
  NumbersDemo()
  {
    mName = "NumbersDemo";
    mNotes = {
      { "<numbers>", "header provides mathematical constants in std::numbers." },
      { "std::numbers::pi", "The mathematical constant pi." },
      { "std::numbers::e", "Euler's number." },
      { "std::numbers::sqrt2", "The square root of two." },
      { "std::numbers::inv_pi", "The inverse of pi." },
      { "std::lerp", "Linearly interpolates between two values." },
      { "std::midpoint",
        "Computes the midpoint between two values without overflow." }
    };
  }

  ~NumbersDemo() = default;

  void ShowExample(
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

    MathConstantsExample();
    LerpExample();
    MidpointExample();
    AngleExample();

    LOG_END_FUNCTION_CLASS();
  }

  void ShowDemo(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowExample(printNotesCallback);
  }

private:
  void MathConstantsExample()
  {
    LOG_START_FUNCTION();

    std::cout << "pi = " << std::numbers::pi << '\n';
    std::cout << "e = " << std::numbers::e << '\n';
    std::cout << "sqrt(2) = " << std::numbers::sqrt2 << '\n';
    std::cout << "inv_pi = " << std::numbers::inv_pi << '\n';
    std::cout << "pi_v<float> = " << std::numbers::pi_v<float> << '\n';

    LOG_END_FUNCTION();
  }

  void LerpExample()
  {
    LOG_START_FUNCTION();

    double start = 10.0;
    double end = 20.0;
    double result = std::lerp(start, end, 0.25);

    std::cout << "lerp(10, 20, 0.25) = " << result << '\n';
    std::cout << "lerp(10, 20, 0.5) = " << std::lerp(start, end, 0.5)
              << '\n';

    LOG_END_FUNCTION();
  }

  void MidpointExample()
  {
    LOG_START_FUNCTION();

    double a = 10.0;
    double b = 30.0;
    double mid = std::midpoint(a, b);

    std::cout << "midpoint(10, 30) = " << mid << '\n';

    LOG_END_FUNCTION();
  }

  void AngleExample()
  {
    LOG_START_FUNCTION();

    double angleDegrees = 180.0;
    double angleRadians = angleDegrees * std::numbers::pi / 180.0;

    std::cout << "180 degrees in radians = " << angleRadians << '\n';
    std::cout << "sin(pi/2) = " << std::sin(std::numbers::pi / 2.0) << '\n';

    LOG_END_FUNCTION();
  }
};
