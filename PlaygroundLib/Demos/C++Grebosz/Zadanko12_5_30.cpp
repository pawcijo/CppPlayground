#include "Zadanko12_5_30.hpp"

#include <iomanip>
#include <map>
#include <vector>

namespace Z_12_5_30
{

// clang-format off
    //simulated user input for chess pieces positions
    const std::vector<std::vector<std::string>> positions{
        {{"A8"},{"B8"},{"C8"},{"D8"},{"E8"},{"F8"},{"G8"},{"H8"}},
        {{"A7"},{"B7"},{"C7"},{"D7"},{"E7"},{"F7"},{"G7"},{"H7"}},
        {{"A6"},{"B6"},{"C6"},{"D6"},{"E6"},{"F6"},{"G6"},{"H6"}},
        {{"A5"},{"B5"},{"C5"},{"D5"},{"E5"},{"F5"},{"G5"},{"H5"}},
        {{"A4"},{"B4"},{"C4"},{"D4"},{"E4"},{"F4"},{"G4"},{"H4"}},
        {{"A3"},{"B3"},{"C3"},{"D3"},{"E3"},{"F3"},{"G3"},{"H3"}},
        {{"A2"},{"B2"},{"C2"},{"D2"},{"E2"},{"F2"},{"G2"},{"H2"}},
        {{"A1"},{"B1"},{"C1"},{"D1"},{"E1"},{"F1"},{"G1"},{"H1"}}
    };

    std::map<char,int> symbol_map{
        {'A',1},
        {'B',2},
        {'C',3},
        {'D',4},
        {'E',5},
        {'F',6},
        {'G',7},
        {'H',8}
    };
// clang-format on

constexpr size_t pice_size_in_memory = sizeof(long);
constexpr size_t pieces_count = 64;

constexpr size_t num_bytes = pieces_count * pice_size_in_memory; // 512 bytes
void Zadanko()
{
  // Allocate memory for the chessboard

  std::byte* szachownica;

  try
  {
    szachownica = new std::byte[num_bytes](); // zero-initialized
  }
  catch (std::bad_alloc& e)
  {
    std::cerr << "Allocation failed: " << e.what() << "\n";
  }

  //test if it is zero
    int counter2 = 1;
  auto start2 = reinterpret_cast<long*>(szachownica);
  for (long* wks = start2;
       wks < (reinterpret_cast<long*>(szachownica) + pieces_count);
       wks++)
  {
    std::cout <<std::setw(4) <<*wks << " ";
    if (counter2 % 8 == 0 && counter2 > 0)
    {
      std::cout << std::endl;
    }
    counter2++;
  }

  for (int i = 0; i < static_cast<int>(positions.size()); ++i)
  {
    auto& current_row = positions[i];
    for (int j = 0; j < static_cast<int>(current_row.size()); ++j)
    {
      int raw = (current_row[j][1] - '0') - 1;
      int column = symbol_map[current_row[j][0]] - 1;
      int pozycja = raw * 8 + column;
      long* wsk = reinterpret_cast<long*>(szachownica);

      //tak
      wsk[pozycja] = static_cast<long>((i * 8) + j);

      std::cout << "Writing for position: " << current_row[j] << "<" << pozycja
                << ">" << std::endl;
    }
  }

  int counter = 1;
  auto start = reinterpret_cast<long*>(szachownica);
  for (long* wks = start;
       wks < (reinterpret_cast<long*>(szachownica) + pieces_count);
       wks++)
  {
    std::cout <<std::setw(4) <<*wks << " ";
    if (counter % 8 == 0 && counter > 0)
    {
      std::cout << std::endl;
    }
    counter++;
  }

  delete[] szachownica;
}
}