#pragma once

#include <cstdarg>
#include <iostream>

class Log {
public:
  template <typename T, typename... Args>
  static inline void Info(T t, Args... args) {
    print("[INFO]:", t, args...);

    std::cout << std::endl;
  }

  template <typename T, typename... Args>
  static inline void Debug(T t, Args... args) {
    print("[DEBUG]:", t, args...);

    std::cout << std::endl;
  }

  template <typename T, typename... Args>
  static inline void Warn(T t, Args... args) {
    print("[WARN]:", t, args...);

    std::cout << std::endl;
  }

  template <typename T, typename... Args>
  static inline void Error(T t, Args... args) {
    print("[ERROR]:", t, args...);

    std::cout << std::endl;
  }

private:
  template <typename T, typename... Args>
  static inline void print(T t, Args... args) {
    std::cout << t;
    
    print(args...);
  }

  template <typename T> static inline void print(T t) { std::cout << t; }
};