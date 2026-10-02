#pragma once

#include <vector>
#include <array>
#include <queue>
#include <map>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <typeinfo>
#include <memory>
#include <string>
#include <string_view>
#include <random>
#include <optional>
#include <format>
#include <numbers>
#include <chrono>

#include "Attribute.h"

#define PI (std::numbers::pi)

using std::vector;
using std::array;
using std::queue;
//using std::map;
using std::unordered_map;
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;
using std::string;
using std::string_view;
using std::u32string;
using std::u32string_view;
using std::chrono::milliseconds;
using std::chrono::seconds;

using namespace std::chrono_literals;

//namespace chrono = std::chrono;

#pragma warning(disable:4100)
#pragma warning(disable:4457)
#pragma warning(disable:4458)
