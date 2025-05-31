#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include <bit>
#include <bitset>
#include <functional>
#include <iostream>

class BitDemo : public DemoBase
{
public:
  BitDemo()
  {
    mName = "BitDemo";
    mNotes = {
      { "<bit>", "header provides functions for bit manipulation." },
      { "std::bit_cast",
        "Allows casting between types without changing the bit "
        "representation." },
      { "std::has_single_bit", "Checks if a value has exactly one bit set." },
      { "std::rotl and std::rotr", "Rotate bits left or right." },
      { "std::countl_zero and std::countr_zero",
        "Count leading or trailing zeros in a value." },
      { "std::bit_width",
        "Returns the number of bits required to represent a value." },
      { "std::popcount", "Counts the number of bits set to 1 in a value." },
      { "std::bit_ceil and std::bit_floor",
        "Finds the next power of two greater than or equal to a value, or the "
        "largest power of two less than or equal to a value." },
      { "std::countr_one", "Counts trailing ones in a value." },
      { "std::countl_one", "Counts leading ones in a value." }
    };
  }

  ~BitDemo() = default;
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

    BitCastExample();
    SingleBitExample();
    RotlRoTrExample();
    CountlZeroExample();
    CountrZeroExample();
    BitWidthExample();
    PopCountExample();
    BitCeilFloorExample();
		CountrOneExample();
		CountlOneExample();

    LOG_END_FUNCTION_CLASS();
  }

  void ShowDemo(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowExample(printNotesCallback);
  };

private:
  void BitCastExample()
  {
    LOG_START_FUNCTION();
    float f = 3.14f;
    uint32_t bits = std::bit_cast<uint32_t>(f);
    std::cout << "Bits of " << f << ": 0x" << std::hex << bits << std::dec
              << std::endl;
    LOG_END_FUNCTION();
  }
  void SingleBitExample()
  {
    LOG_START_FUNCTION();
    uint value = 0b00000001; // 1 in binary
    if (std::has_single_bit(value))
    {
      std::cout << "Value has exactly one bit set." << std::endl;
    }
    else
    {
      std::cout << "Value does not have exactly one bit set." << std::endl;
    }

    uint value2 = 0b00000011; // 1 in binary
    if (std::has_single_bit(value2))
    {
      std::cout << "Value2 has exactly one bit set." << std::endl;
    }
    else
    {
      std::cout << "Value2 does not have exactly one bit set." << std::endl;
    }

    LOG_END_FUNCTION();
  }

  void RotlRoTrExample()
  {
    LOG_START_FUNCTION();
    uint32_t value = 0b00000010; // 1 in binary
    std::cout << "Value: 0b" << std::bitset<8>(value) << std::endl;

    uint32_t rotatedLeft = std::rotl(value, 1);
    std::cout << "Rotated Left: 0b" << std::bitset<8>(rotatedLeft) << std::endl;
    std::cout << std::endl;

    uint32_t value2 = 0b00000010; // 1 in binary
    std::cout << "Value 2: 0b" << std::bitset<8>(value2) << std::endl;

    // Rotate right by 1 bit

    uint32_t rotatedRight = std::rotr(value2, 1);
    std::cout << "Value 2 Rotated Right: 0b" << std::bitset<8>(rotatedRight)
              << std::endl;
    LOG_END_FUNCTION();
  }

  void CountlZeroExample()
  {
    LOG_START_FUNCTION();
    uint8_t value = 0b00001111; // 15 in binary
    uint8_t leadingZeros = std::countl_zero(value);
    std::cout << "Leading zeros in " << std::bitset<8>(value) << ": "
              << static_cast<int>(leadingZeros) << std::endl;

    uint8_t value2 = 0b11110000; // 240 in binary
    uint8_t leadingZeros2 = std::countl_zero(value2);
    std::cout << "Leading zeros in " << std::bitset<8>(value2) << ": "
              << static_cast<int>(leadingZeros2) << std::endl;
    LOG_END_FUNCTION();
  }
  void CountrZeroExample()
  {
    LOG_START_FUNCTION();
    uint8_t value = 0b00001111; // 15 in binary
    uint8_t trailingZeros = std::countr_zero(value);
    std::cout << "Trailing zeros in " << std::bitset<8>(value) << ": "
              << static_cast<int>(trailingZeros) << std::endl;

    uint8_t value2 = 0b11110000; // 240 in binary
    uint8_t trailingZeros2 = std::countr_zero(value2);
    std::cout << "Trailing zeros in " << std::bitset<8>(value2) << ": "
              << static_cast<int>(trailingZeros2) << std::endl;
    LOG_END_FUNCTION();
  }

  void BitWidthExample()
  {
    LOG_START_FUNCTION();
    uint32_t value = 0b00001111; // 15 in binary
    uint32_t width = std::bit_width(value);
    std::cout << "Bit width of " << std::bitset<8>(value) << ": " << width
              << std::endl;

    uint32_t value2 = 0b11110000; // 240 in binary
    uint32_t width2 = std::bit_width(value2);
    std::cout << "Bit width of " << std::bitset<8>(value2) << ": " << width2
              << std::endl;

    uint32_t value3 = 0b01110000; // 240 in binary
    uint32_t width3 = std::bit_width(value3);
    std::cout << "Bit width of " << std::bitset<8>(value3) << ": " << width3
              << std::endl;

    LOG_END_FUNCTION();
  }

  void PopCountExample()
  {
    LOG_START_FUNCTION();
    uint32_t value = 0b00001111; // 15 in binary
    uint32_t count = std::popcount(value);
    std::cout << "Number of bits set in " << static_cast<int>(value) << " ("
              << std::bitset<8>(value) << "): " << count << std::endl;

    uint32_t value2 = 0b11110000; // 240 in binary
    uint32_t count2 = std::popcount(value2);
    std::cout << "Number of bits set in " << static_cast<int>(value2) << " ("
              << std::bitset<8>(value2) << "): " << count2 << std::endl;

    uint32_t value3 = 0b10010001; // 145 in binary
    uint32_t count3 = std::popcount(value3);
    std::cout << "Number of bits set in " << static_cast<int>(value3) << " ("
              << std::bitset<8>(value3) << "): " << count3 << std::endl;

    LOG_END_FUNCTION();
  }

  void BitCeilFloorExample()
  {
    LOG_START_FUNCTION();
    uint32_t value = 5; // 5 in binary is 101
    std::cout << "Value: " << std::bitset<8>(value) << std::endl;
    uint32_t ceil = std::bit_ceil(value);
    uint32_t floor = std::bit_floor(value);
    std::cout << "Bit ceil of " << value << ": " << ceil << std::endl;
    std::cout << "Bit floor of " << value << ": " << floor << std::endl;

    uint32_t value2 = 8; // 8 in binary is 1000
    std::cout << "Value2: " << std::bitset<8>(value2) << std::endl;
    uint32_t ceil2 = std::bit_ceil(value2);
    uint32_t floor2 = std::bit_floor(value2);
    std::cout << "Bit ceil of " << value2 << ": " << ceil2 << std::endl;
    std::cout << "Bit floor of " << value2 << ": " << floor2 << std::endl;

    LOG_END_FUNCTION();
  };

  void CountrOneExample()
  {
    LOG_START_FUNCTION();
    uint32_t value = 0b00001111; // 15 in binary
    uint32_t trailingOnes = std::countr_one(value);
    std::cout << "Trailing ones in " << std::bitset<8>(value) << ": "
              << static_cast<int>(trailingOnes) << std::endl;

    uint32_t value2 = 0b11110000; // 240 in binary
    uint32_t trailingOnes2 = std::countr_one(value2);
    std::cout << "Trailing ones in " << std::bitset<8>(value2) << ": "
              << static_cast<int>(trailingOnes2) << std::endl;
    LOG_END_FUNCTION();
  };
  void CountlOneExample()
  {
    LOG_START_FUNCTION();
    uint8_t value = 0b00001111; // 15 in binary
    uint8_t leadingOnes = std::countl_one(value);
    std::cout << "Leading ones in " << std::bitset<8>(value) << ": "
              << static_cast<int>(leadingOnes) << std::endl;

    uint8_t value2 = 0b11110000; // 240 in binary
    uint8_t leadingOnes2 = std::countl_one(value2);
    std::cout << "Leading ones in " << std::bitset<8>(value2) << ": "
              << static_cast<int>(leadingOnes2) << std::endl;
    LOG_END_FUNCTION();
  };
};
