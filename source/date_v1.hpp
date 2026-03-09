#pragma once

#include <compare>

namespace date_v1 {

struct Date {
  int year;
  int month;
  int day;

  auto operator<=>(const Date &) const = default;
};

} // namespace date_v1
