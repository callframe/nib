#pragma once

#include <concepts>

namespace nib
{

template <typename F>
concept Defer_Callback = requires(F f) {
  { f() } -> std::same_as<void>;
};

template <Defer_Callback F>
class Defer
{
 public:
  Defer(F f) : f(f) {}
  ~Defer() { f(); }

  Defer(const Defer&) = delete;
  Defer& operator=(const Defer&) = delete;

  Defer(Defer&&) = default;
  Defer& operator=(Defer&&) = delete;

  F f;
};

}  // namespace nib

#define NIB_DEFER(__code) auto NIB_LOCAL_UNIQUE(_defer_) = nib::Defer([&]() { __code; })
