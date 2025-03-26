#include <iostream>

#include "Common/DemoBase.hpp"
#include "Common/DemoFactory.hpp"

#include <memory>
#include <ostream>
#include <stdexcept>

using namespace std;

int main(int argc, char* argv[])
{

  int DemoSize = static_cast<int>(DemoType::Count);
  std::unique_ptr<DemoBase> demo;

  auto demoMap = DemoFactory::getDemoMap();

  if (argc < 2)
  {
    std::cerr << "Please select demo from  1 to " << (DemoSize - 1) << "."
              << std::endl;

    int counter = 1;
    for (const auto& value : demoMap)
    {
      std::cout << counter << " : " << value.second.second << std::endl;
      counter++;
    }

    return 1;
  }

  int num = -1;
  try
  {
    num = std::stoi(argv[1]);

    // double check
    std::string test = argv[1];
    if (!is_string_only_numeric(test))
    {
      std::string message =
        "Nice try, but " + test + " is not in right format.";
      throw std::invalid_argument(message);
    }
  }
  catch (std::invalid_argument& exception)
  {
    std::cerr << "Wrong number." << "\nPlease pass numeric value." << std::endl;
    std::cerr << "ERROR: " << exception.what() << std::endl;
    return 1;
  }

  if (static_cast<int>(num) >= DemoSize || static_cast<int>(num) <= 0)
  {
    std::cerr << "Wrong number." << "\nPlease select demo from  1 to "
              << DemoSize - 1 << "." << std::endl;
    return 1;
  }

  cout << "\033[1;32m- - - - - - - - -Modern Cpp- - - - - - - - - - - \033[0m\n"
       << std::endl;

  demo = DemoFactory::createDemo(static_cast<DemoType>(num));

  if (nullptr != demo)
  {
    demo->ShowDemo();
  }
  else
  {
    std::cout << "Demo not implemnted yet \n";
  }

  return 0;
}
