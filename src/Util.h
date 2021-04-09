#pragma once

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <vector>
#include <sstream>

#define UNUSED(x) (void)x

// FOREACH Enum Helpers
#define ENUM(x) x
#define STRING(x) #x

// Logging Tools
#include "Logging.h"

#define ASSERT(condition, message) do {                                        \
  if (!(condition)) {                                                          \
    LOG_ERROR("Assertion " << #condition << " failed at " << __FILE__          \
                           << ":" << __LINE__ << " " << message);              \
  }                                                                            \
} while (0)

template<typename T>
bool Contains(const std::vector<T> vec, T elem) {
    return std::find(vec.begin(), vec.end(), elem) != vec.end();
}

template<typename T>
bool Contains(const std::vector<T> vec, std::initializer_list<T> elems) {
    bool ret = true;
    for (auto elem: elems) {
        ret = ret && Contains(vec, elem);
    }
    return ret;
}

template<typename T>
bool ContainsAny(std::vector<T> vec, std::initializer_list<T> elems) {
    for (auto elem: elems) {
        if (Contains(vec, elem))
            return true;
    }
    return false;
}

struct pair_hash {
  template<class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &pair) const {
    return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
  }
};

struct VectorHasher {
  template<class T>
    int operator()(const std::vector<T> &V) const {
        int hash = V.size();
        for(auto &i : V) {
          hash ^= std::hash<T>(i);
        }
        return hash;
    }
};

template<typename K>
std::ostream& operator<<(std::ostream& out, const std::unordered_set<K>& set);

template<typename K, typename V>
std::ostream& operator<<(std::ostream& out, const std::unordered_map<K,V>& map) {
    out << "{" << std::endl;
    for (const auto& pair : map) {
        out << pair.first << ": " << pair.second << std::endl;
    }
    out << "}";
    return out;
}

template<typename K>
std::ostream& operator<<(std::ostream& out, const std::unordered_set<K>& set) {
    out << "{";
    for (const auto& elem : set) {
        out << elem << ",";
    }
    out << "}";
    return out;
}

// Whitespace Trimming (https://stackoverflow.com/a/217605/1834967)
static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
    return !std::isspace(ch);
  }));
}

static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
    return !std::isspace(ch);
  }).base(), s.end());
}

static inline void trim(std::string &s) {
  ltrim(s);
  rtrim(s);
}

static inline std::string ltrim_copy(std::string s) {
  ltrim(s);
  return s;
}

static inline std::string rtrim_copy(std::string s) {
  rtrim(s);
  return s;
}

static inline std::string trim_copy(std::string s) {
  trim(s);
  return s;
}

template <typename T>
static inline std::string ListToString(
    const T& list,
    const std::string& delim) {
  std::ostringstream str;
  bool first = true;
  for (const auto& elem : list) {
    if (!first) str << delim;
    first = false;
    str << elem;
  }
  return str.str();
}

template <typename T>
static inline std::string ListToString(
    const std::vector<std::unique_ptr<T>>& list,
    const std::string& delim) {
  std::ostringstream str;
  bool first = true;
  for (const auto& elem : list) {
    if (!first) str << delim;
    first = false;
    str << *elem.get();
  }
  return str.str();
}

template <typename T>
static inline std::string ListToString(
    const std::vector<T*>& list,
    const std::string& delim) {
  std::ostringstream str;
  bool first = true;
  for (const auto& elem : list) {
    if (!first) str << delim;
    first = false;
    str << *elem;
  }
  return str.str();
}

static inline std::string CharToString(const char c) {
  if (std::isprint(c)) return std::string(1, c);
  if (c == '\n') return "\\n";
  if (c == '\r') return "\\r";
  if (c == '\t') return "\\t";
  if (c == '\f') return "\\f";
  if (c == '\b') return "\\b";
  return "ASCII-" + std::to_string((int) c);
}

template <typename T>
class HasToString {
private:
    typedef char YesType[1];
    typedef char NoType[2];

    template <typename C> static YesType& test(decltype(&C::ToString));
    template <typename C> static NoType& test(...);
public:
    enum { value = sizeof(test<T>(0)) == sizeof(YesType) };
};

// Automatically define a operator<< for stream output if the class has a 
//   ToString() method
template<class T>
typename std::enable_if<HasToString<T>::value, std::ostream&>::type 
operator<<(std::ostream& out, const T& obj) {
  out << obj.ToString();
  return out;
}