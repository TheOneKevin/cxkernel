#include <ebl/assert.h>
#include <iostream>
#include <exception>

void assert(bool condition, std::source_location const &loc) {
  if (!condition) [[unlikely]] {
    std::cout << "Assertion failed at " << loc.file_name() << ":" << loc.line()
              << " " << loc.function_name() << std::endl;
    throw std::exception();
  }
}

void assert(bool condition, const char *message,
            std::source_location const &loc) {
  if (!condition) [[unlikely]] {
    std::cout << "Assertion failed at " << loc.file_name() << ":" << loc.line()
              << " " << loc.function_name() << std::endl;
    throw std::exception();
  }
}

void panic(const char *message, std::source_location const &loc) {
  std::cout << "Kernel panicked at " << loc.file_name() << ":" << loc.line()
            << " " << loc.function_name() << std::endl;
  throw std::exception();
}
