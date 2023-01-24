#include "search.hpp"

std::vector <unsigned int> ZFunction(const std::string &string) {
  unsigned int length = string.size();
  std::vector <unsigned int> ZFunctionVector(length);
  unsigned int ThirdIndex = 0, FourthIndex = 0;

  for(unsigned int index = 1; index < length; ++index) {
    if(index <= FourthIndex) {
      ZFunctionVector[index] = std::min(ZFunctionVector[index - ThirdIndex], FourthIndex - index);
    }

    while(index + ZFunctionVector[index] < length and string[index + ZFunctionVector[index]] == string[ZFunctionVector[index]]) {
      ++ZFunctionVector[index];
    }

    if(index + ZFunctionVector[index] > FourthIndex) {
      ThirdIndex = index;
      FourthIndex = index + ZFunctionVector[index];
    }
  }

  return ZFunctionVector;
}

std::vector <unsigned int> PrefixFunction(const std::string &string) {
  std::vector <unsigned int> ZFunctionVector = ZFunction(string);
  unsigned int length = string.size();
  std::vector <unsigned int> PrefixVector(length);
    
  for(unsigned int index = length - 1; index > 0; --index) {
    PrefixVector[index + ZFunctionVector[index] - 1] = ZFunctionVector[index];
  }

  return PrefixVector;
}

std::vector <unsigned int> KMPFunction(const std::string &pattern, const std::string &text) {
  std::vector <unsigned int> vector = PrefixFunction(pattern);
  unsigned int PatternSize = pattern.size();
  unsigned int length = text.size();
  unsigned int index = 0;
  std::vector <unsigned int> carrier;

  if(PatternSize > length) {
    return carrier;
  }

  while(index < length - PatternSize + 1) {
    unsigned int SecondIndex = 0;

    while(SecondIndex < PatternSize and pattern[SecondIndex] == text[index + SecondIndex]) {
      ++SecondIndex;
    }

    if(SecondIndex == PatternSize) {
      carrier.push_back(index);
    } 
    else {
      if(SecondIndex > 0 and SecondIndex > vector[SecondIndex - 1]) {
        index = index + SecondIndex - vector[SecondIndex - 1] - 1;
      }
    }
    
    ++index;
  }

  return carrier;
}
