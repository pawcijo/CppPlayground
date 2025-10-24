#include "Zadanko14_7_16.hpp"

#include <functional>
#include <print>
#include <thread>
#include <vector>
// Robot marsjański

auto zgiecie_ramienia(double stopnie)
{
  std::println("{} o {} stopni", __func__, stopnie);
  bool value = true;

  return value;
}
auto zgiecie_nadgarska(double stopnie)
{
  std::println("{} o {} stopni", __func__, stopnie);
  bool value = true;

  return value;
}
auto zgiecie_przedramienia(double stopnie)
{
  std::println("{} o {} stopni", __func__, stopnie);
  bool value = true;

  return value;
}
auto obrot_ramienia(double stopnie)
{
  std::println("{} o {} stopni", __func__, stopnie);
  bool value = true;

  return value;
}
auto obrot_nadgarska(double stopnie)
{
  std::println("{} o {} stopni", __func__, stopnie);
  bool value = true;

  return value;
}
auto obrot_przedramienia(double stopnie)
{
  std::println("{} o {} stopni", __func__, stopnie);
  bool value = true;

  return value;
}

std::vector<std::function<bool(double)>> sterowanie_robotem = {
  nullptr,        zgiecie_ramienia, zgiecie_nadgarska,  zgiecie_przedramienia,
  obrot_ramienia, obrot_nadgarska,  obrot_przedramienia
};

enum class ESterowanieRobotem
{
  koniec = 0,
  Zgiecie_ramienia = 1,
  Zgiecie_nadgarska,
  Zgiecie_przedramienia,
  Obrot_ramienia,
  Obrot_nadgarska,
  Obrot_przedramienia

};

//Not error prone
void Zadanko14_7_16::Zadanko()
{

  std::thread watek(
    [&]()
    {
      bool work = true;

      while (work)
      {
        std::println("Co mam robic? : (1 - 6) i ile stopni ");
        int what = 0;
        double ile_stopni{};
        std::cin >> what >> ile_stopni;

        if (what > 0 && what < 7)
        {
          auto action = sterowanie_robotem[what];

          if (nullptr != action)
          {
            action(ile_stopni);
          }
        }
        else
        {

          work = false;
        }
      }
      std::println("Koniec");
    });

  // do not wait for synchronization
  watek.detach();
}