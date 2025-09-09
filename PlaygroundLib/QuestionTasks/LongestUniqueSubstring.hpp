
#pragma once
#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include <unordered_set>
#include <string>
#include <iostream>

class Solution {
public:
     int lengthOfLongestSubstring(std::string& str) {
        int n = str.length();
        if (n == 0) return 0;

        int maxLen = 0;
        int left = 0;  // window start
        std::unordered_set<char> seen;
        std::string best;

        for (int right = 0; right < n; ++right) 
        {
            char character = str[right];
            //Move left to right if characters are duplicated
            while (seen.contains(character)) 
            {
                seen.erase(str[left]);
                left++;
            }
           seen.insert(character);

            //L|-------|R---
            //check window size is bigger 
           if(right - left +1 >maxLen)
           {
              maxLen = right - left + 1;
              best = str.substr(left,maxLen);
           }
        }

        str = best; // store longest substring back in str
        return maxLen;
    }
};


class LongestUniqueSubstring : public DemoBase
{
public:
  LongestUniqueSubstring()
  {
    mName = "LongestUniqueSubstring";
    mNotes = {
      { "LongestUniqueSubstring:",
        R"(A task to find the longest substring without repeating characters in a string.)" }
    };
  }
    void ShowDemo(
        std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
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

        std::string test1 = "abcabcbb";
        std::string test2 = "bbbbb";
        std::string test3 = "pwwkew";
        std::string test4 = "";
        std::string test5 = "au";
        std::string test6 = "dvdf";

        Solution solution;

        int len1 = solution.lengthOfLongestSubstring(test1);
        int len2 = solution.lengthOfLongestSubstring(test2);
        int len3 = solution.lengthOfLongestSubstring(test3);
        int len4 = solution.lengthOfLongestSubstring(test4);
        int len5 = solution.lengthOfLongestSubstring(test5);
        int len6 = solution.lengthOfLongestSubstring(test6);

        std::cout << "Test 1: Input: \"abcabcbb\" | Longest Substring: \"" << test1
                  << "\" | Length: " << len1 << std::endl;
        std::cout << "Test 2: Input: \"bbbbb\" | Longest Substring: \"" << test2
                  << "\" | Length: " << len2 << std::endl;
        std::cout << "Test 3: Input: \"pwwkew\" | Longest Substring: \"" << test3
                  << "\" | Length: " << len3 << std::endl;
        std::cout << "Test 4: Input: \"\" | Longest Substring: \"" << test4
                  << "\" | Length: " << len4 << std::endl;
        std::cout << "Test 5: Input: \"au\" | Longest Substring: \"" << test5
                  << "\" | Length: " << len5 << std::endl;
        std::cout << "Test 6: Input: \"dvdf\" | Longest Substring: \"" << test6
                  << "\" | Length: " << len6 << std::endl;
    }

};
