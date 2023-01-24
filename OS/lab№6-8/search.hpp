#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <string>
#include <vector>

std::vector <unsigned int> ZFunction(const std::string &string);
std::vector <unsigned int> PrefixFunction(const std::string &string);
std::vector <unsigned int> KMPFunction(const std::string &pattern, const std::string &text);

#endif
