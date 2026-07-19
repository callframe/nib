#pragma once

#include <utility>

namespace nib
{

/**
 * @brief Represents a successful result with a value of type T.
 * @tparam T The type of the success value.
 */
template <typename T>
struct Ok
{
  T value;
};

/**
 * @brief Represents an error result with a value of type E.
 * @tparam E The type of the error value.
 */
template <typename E>
struct Err
{
  E value;
};

/**
 * @brief Result type for carrying success or error values.
 * @tparam T The type of the success value.
 * @tparam E The type of the error value.
 */
template <typename T, typename E>
class Result
{
 public:
  Result(Ok<T> ok) : is_ok(true) { new (&this->ok) T(std::move(ok.value)); }
  Result(Err<E> err) : is_ok(false) { new (&this->err) E(std::move(err.value)); }

  Result(const Result& other) = delete;
  Result& operator=(const Result& other) = delete;

  Result(Result&& other) noexcept : is_ok(other.is_ok)
  {
    if (this->is_ok)
    {
      new (&this->ok) T(std::move(other.ok));
    }
    else
    {
      new (&this->err) E(std::move(other.err));
    }
    other.is_ok = false;
  }

  Result& operator=(Result&& other) noexcept
  {
    if (this != &other)
    {
      this->~Result();
      new (this) Result(std::move(other));
    }
    return *this;
  }

  ~Result()
  {
    if (this->is_ok)
    {
      this->ok.~T();
    }
    else
    {
      this->err.~E();
    }
  }

  bool is_ok;
  union
  {
    T ok;
    E err;
  };
};

}  // namespace nib
