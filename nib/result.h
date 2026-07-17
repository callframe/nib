#pragma once

#include <cstdint>
#include <stdexcept>
#include <utility>

namespace nib
{

enum class Result_State : uint8_t
{
  Empty,
  Ok,
  Err,
};

template <typename T, typename E>
class Result
{
 public:
  Result() : state(Result_State::Empty) {}
  Result(T ok) : state(Result_State::Ok), ok(std::move(ok)) {}
  Result(E err) : state(Result_State::Err), err(std::move(err)) {}

  Result(const Result& other) = delete;
  Result& operator=(const Result& other) = delete;

  Result(Result&& other) noexcept : state(other.state)
  {
    if (state == Result_State::Ok)
    {
      new (&ok) T(std::move(other.ok));
    }
    else if (state == Result_State::Err)
    {
      new (&err) E(std::move(other.err));
    }
    other.state = Result_State::Empty;
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
    if (state == Result_State::Ok)
    {
      ok.~T();
    }
    else if (state == Result_State::Err)
    {
      err.~E();
    }
  }

  operator bool() const { return state == Result_State::Ok; }

  T unwrap()
  {
    if (this->state != Result_State::Ok)
    {
      throw std::runtime_error("Result is not Ok");
    }

    T value(std::move(this->ok));
    this->ok.~T();
    this->state = Result_State::Empty;

    return value;
  }

  E unwrap_err()
  {
    if (this->state != Result_State::Err)
    {
      throw std::runtime_error("Result is not Err");
    }

    E value(std::move(this->err));
    this->err.~E();
    this->state = Result_State::Empty;

    return value;
  }

 private:
  Result_State state;
  union
  {
    T ok;
    E err;
  };
};

}  // namespace nib