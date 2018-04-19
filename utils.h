#include <string> 

extern void ltrim(std::string &s);

// trim from end (in place)
extern void rtrim(std::string &s);

extern void trim(std::string &s);

// trim (copying)
extern std::string trim_copy(std::string s);