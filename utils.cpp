#include <algorithm> 
#include <cctype>
#include <locale>

#include "utils.h"

void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
        [](int ch)
        {
            return !std::isspace(ch);
        }));
}

// trim from end (in place)
void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
        [](int ch)
        {
            return !std::isspace(ch);
        }).base(), s.end());
}

// trim from both ends (in place)
void trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
std::string trim_copy(std::string s)
{
    trim(s);
    return s;
}
