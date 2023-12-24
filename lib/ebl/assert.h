#pragma once

#include <ebl/source_location.h>

void assert(bool condition, std::source_location const& l = std::source_location::current());
void assert(bool condition,
            const char* message,
            std::source_location const& l = std::source_location::current());
void panic(const char* message, std::source_location const& l = std::source_location::current());
