#pragma once

#include <iostream>

// #define __LOGGER_ENABLE

#ifdef __LOGGER_ENABLE
#define LOGGER(VAR) std::cout << VAR
#else
#define LOGGER(VAR)
#endif

#define LOGGER_ENDL() LOGGER(std::endl)
#define LOGGER_PRINT(VAR) LOGGER(VAR); LOGGER_ENDL()
#define LOGGER_PRINT_VAR(VAR) LOGGER_PRINT(#VAR << ": " << VAR)