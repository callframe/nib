#pragma once

#include <cstdint>
#include <glm/glm.hpp>

namespace nib
{

constexpr inline char APP_NAME[] = "nib";
constexpr glm::vec2 APP_WINDOW_SIZE = glm::ivec2(800, 600);

constexpr std::uint32_t APP_VERSION_MAJOR = 0;
constexpr std::uint32_t APP_VERSION_MINOR = 1;
constexpr std::uint32_t APP_VERSION_PATCH = 0;

#define NIB_CONCAT2(__x, __y) __x##__y
#define NIB_CONCAT(__x, __y) NIB_CONCAT2(__x, __y)

#define NIB_LOCAL_UNIQUE(__stem) NIB_CONCAT(__stem, __LINE__)

struct Unit
{};

}  // namespace nib
