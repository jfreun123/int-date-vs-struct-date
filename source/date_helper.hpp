#pragma once

#include "date_common_functions.hpp"

namespace date_helper {

constexpr int year(int date) { return date / 10000; }
constexpr int month(int date) { return (date / 100) % 100; }
constexpr int day(int date) { return date % 100; }

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
  int y = year(date);
  int m = month(date);
  int d = day(date);

  // Sun–Thu → 1, Fri → 3 (skip Sat+Sun), Sat → 2 (skip Sun)
  constexpr int kAdvance[7] = {1, 1, 1, 1, 1, 3, 2}; // Sun=0 … Sat=6
  const int advance =
      kAdvance[static_cast<int>(date_common_functions::dayOfWeek(y, m, d))];

  for (int i = 0; i < advance; ++i) {
    if (d < date_common_functions::daysInMonth(y, m)) {
      ++d;
    } else if (m < 12) {
      ++m;
      d = 1;
    } else {
      ++y;
      m = 1;
      d = 1;
    }
  }

  return pack(y, m, d);
}

} // namespace date_helper
