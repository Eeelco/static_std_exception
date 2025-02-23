#ifndef EXCEPTION_TYPES_H_
#define EXCEPTION_TYPES_H_

#include <cstring>
#include <stdexcept>

constexpr std::size_t stackBufferSize = 256;
const char truncatedMessage[] = "...<truncated>";
constexpr std::size_t truncatedMessageSize = sizeof(truncatedMessage);

static_assert(truncatedMessageSize < stackBufferSize);

class StackString {
public:
  StackString(const char *str) {
    std::size_t len = std::strlen(str);
    if (len >= stackBufferSize) {
      std::strncpy(buffer, str, stackBufferSize - truncatedMessageSize);
      std::strncpy(buffer + stackBufferSize - truncatedMessageSize, truncatedMessage,
                  truncatedMessageSize);
      buffer[stackBufferSize-1] = '\0';
    } else {
      std::strncpy(buffer, str, len);
      buffer[len] = '\0';
    }
  }
  StackString(const StackString &other) {
    std::strncpy(buffer, other.buffer, stackBufferSize);
  }
  StackString(StackString &&other) {
    std::strncpy(buffer, other.buffer, stackBufferSize);
  }

  const char* c_str() const { return buffer; }

private:
  char buffer[stackBufferSize];
};

template <typename Base>
class Exception : public Base {
public:
  Exception(const char *msg) : Base(""), message(msg) {}

  const char *what() const noexcept override { return message.c_str(); }
private:
  StackString message;
};

using LogicError = Exception<std::logic_error>;

using DomainError = Exception<std::domain_error>;

using InvalidArgument = Exception<std::invalid_argument>;

using LengthError = Exception<std::length_error>;

using OutOfRange = Exception<std::out_of_range>;

using RuntimeError = Exception<std::runtime_error>;

using RangeError = Exception<std::range_error>;

using OverflowError = Exception<std::overflow_error>;

using UnderflowError = Exception<std::underflow_error>;

#endif