#pragma once

// -----------------------
// MACROS
// -----------------------

#include "../converters/converters_pch.h"

#define INIT_ARR(arr, size) arr = new std::remove_reference<decltype(*arr)>::type[size];
#define CONVERTIBLE_ENTITY friend class GLOBAL_MAPPER_CLASS;