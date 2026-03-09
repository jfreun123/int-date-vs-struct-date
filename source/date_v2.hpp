#pragma once

#include <compare>

namespace date_v2 {

struct Date {
  short year;
  char month;
  char day;

  auto operator<=>(const Date &) const = default;
};

} // namespace date_v2
