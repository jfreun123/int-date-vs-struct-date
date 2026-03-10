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
  // Unpack once — all arithmetic stays at component level.
  int y = year(date);
  int m = month(date);
  int d = day(date);

  // Days to advance based on today's DOW:
  //   Sun–Thu → 1  (next day is Mon–Fri, a weekday)
  //   Fri     → 3  (skip Sat+Sun, land on Mon)
  //   Sat     → 2  (skip Sun, land on Mon)
  // Lookup avoids branching on the common Mon–Thu path.
  constexpr int kAdvance[7] = {1, 1, 1, 1, 1, 3, 2}; // Sun=0 … Sat=6
  const int advance =
      kAdvance[static_cast<int>(date_common_functions::dayOfWeek(y, m, d))];

  // Advance `advance` days without repacking or re-unpacking.
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
