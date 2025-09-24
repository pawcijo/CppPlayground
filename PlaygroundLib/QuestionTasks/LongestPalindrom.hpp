#pragma once
#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include <iostream>
#include <string>
#include <functional>

namespace {
class SolutionLongestPalindrom 
{
public:
      static bool checkIfPalindrom(std::string& aString) {

        std::cout<<"Check if palindrom :"<<aString<<"\n";

        if(aString.empty())
        {
            return false;
        }
        if(aString.length() == 1)
        {
            return true;
        }
        int aHalfSize = aString.length() / 2;
        int length = aString.length();
        for (int i = 0; i < aHalfSize; ++i) {
            if (aString[i] != aString[length - i - 1]) {
                return false;
            }
        }
        return true;
    }

    //Slow brute force solution O(n^3)
    static std::string longestPalindrome(std::string input) 
    {
        std::string bigestPalindrom{};
        int inputLenth = input.length();

        if (inputLenth == 1) {
            return input;
        }

        for (int i = 0; i < inputLenth;++i)
        {
            char testChar = input[i];
               //std::cout<<"index i :"<<i<<" "<<testChar<<"\n";
            for (int j = i+1; j < inputLenth;++j)
            {
                //std::cout<<"index j :"<<j<<"\n";
                if(testChar == input[j])            
                {   
                    //std::cout<<"Same char found at index j :"<<j<<"\n";
                    std::string posiblePalindrom;
                    posiblePalindrom+=input[j];
                    for(int k = i+1;k<j+1;k++)
                    {
                        //std::cout<<"index k "<<k<<" "<<input[k]<<"\n";
                        posiblePalindrom+=input[k];
                    }
                    if(checkIfPalindrom(posiblePalindrom))
                    {
                        if(posiblePalindrom.length() > bigestPalindrom.length())
                        bigestPalindrom = posiblePalindrom;
                    }
                    posiblePalindrom.clear();
                }
            }
        }
        return bigestPalindrom;
    }

    //faster solution 0(n^2)

    static std::string longestPalindromeFaster(std::string s) 
    {
        int n = s.length();
        if (n == 0) return "";

        int start = 0, maxLength = 1;

        for (int i = 0; i < n; ++i) 
        {
            // Odd length palindromes
            int left = i, right = i;
            while (left >= 0 && right < n && s[left] == s[right]) 
            {
                if (right - left + 1 > maxLength) 
                {
                    start = left;
                    maxLength = right - left + 1;
                }
                --left;
                ++right;
            }

            // Even length palindromes
            left = i, right = i + 1;
            while (left >= 0 && right < n && s[left] == s[right]) 
            {
                if (right - left + 1 > maxLength) 
                {
                    start = left;
                    maxLength = right - left + 1;
                }
                --left;
                ++right;
            }
        }

        return s.substr(start, maxLength);
    }
};
}



class LongestPalindrom : public DemoBase
{
public:
  LongestPalindrom()
  {
    mName = "LongestPalindrom";
    mNotes = {
      { "LongestPalindrom:",
        R"(A task to find the longest palindromic substring in a given string.)" }
    };
    }

    void ShowDemo(std::function<void (NoteFormat &)> printNotesCallback = nullptr) override
    {
        ShowExample(printNotesCallback);
    }

    void ShowExample(std::function<void (NoteFormat &)> printNotesCallback = nullptr) override
    {
        if (nullptr == printNotesCallback)
        {
            PrintNotes();
        }
        else
        {
            printNotesCallback(mNotes);
        }

        std::string testString = "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";
        std::cout<<"Test string :"<<testString<<"\n";
        std::string result = SolutionLongestPalindrom::longestPalindrome(testString);
        std::cout<<"Longest palindrom in string is :"<<result<<"\n";

    }
};