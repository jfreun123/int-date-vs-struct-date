#pragma once

#include "date_common_functions.hpp"

namespace date_helper {

// Extract components from a yyyymmdd int
constexpr int year(int date) { return date / 10000; }
constexpr int month(int date) { return (date / 100) % 100; }
constexpr int day(int date) { return date % 100; }

// Pack components back into yyyymmdd
constexpr int pack(int y, int m, int d) { return y * 10000 + m * 100 + d; }

constexpr date_common_functions::DayOfWeek dayOfWeek(int date) {
  return date_common_functions::dayOfWeek(year(date), month(date), day(date));
}

constexpr int nextDay(int date) {
  const int y = year(date);
  const int m = month(date);
  const int d = day(date);

  if (d < date_common_functions::daysInMonth(y, m)) {
    return pack(y, m, d + 1);
  }
  if (m < 12) {
    return pack(y, m + 1, 1);
  }
  return pack(y + 1, 1, 1);
}

constexpr int getNextWeekday(int date) {
  const int next = nextDay(date);
  const date_common_functions::DayOfWeek dow = dayOfWeek(next);
  if (dow == date_common_functions::DayOfWeek::Sunday) {
    return nextDay(next);
  }
  if (dow == date_common_functions::DayOfWeek::Saturday) {
    return nextDay(nextDay(next));
  }
  return next;
}

} // namespace date_helper
