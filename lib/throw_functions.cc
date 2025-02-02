#include "exception_types.h"
#include <cstdarg>

namespace std {
void __throw_logic_error(const char *__s) { throw LogicError(__s); }

void __throw_domain_error(const char *__s) { throw DomainError(__s); }

void __throw_invalid_argument(const char *__s) { throw InvalidArgument(__s); }

void __throw_length_error(const char *__s) { throw LengthError(__s); }

void __throw_out_of_range(const char *__s) { throw OutOfRange(__s); }

void __throw_out_of_range_fmt(const char *__fmt, ...) {
  va_list args;
  va_start(args, __fmt);
  char buffer[stackBufferSize];
  vsnprintf(buffer, sizeof(buffer), __fmt, args);
  va_end(args);
  throw OutOfRange(buffer);
}

void __throw_runtime_error(const char *__s) { throw RuntimeError(__s); }

void __throw_range_error(const char *__s) { throw RangeError(__s); }

void __throw_overflow_error(const char *__s) { throw OverflowError(__s); }

void __throw_underflow_error(const char *__s) { throw UnderflowError(__s); }

} // namespace std