#pragma once

#include <array>

namespace date_common_functions {

enum class DayOfWeek {
  Sunday = 0,
  Monday,
  Tuesday,
  Wednesday,
  Thursday,
  Friday,
  Saturday,
};

// Returns the day of week for a given Gregorian date.
//
// Treats the year as starting March 1 so leap-day (Feb 29) always falls at
// the very end — the leap-year correction then applies to a complete year
// with no special-casing needed.
//
// daysFromMarch1[] is a prefix sum: cumulative days from March 1 to the
// first day of each month (Mar=0, Apr=31, May=61, …, Jan=306, Feb=337).
// Only the value mod 7 matters for day-of-week arithmetic.
//
// The true day count from the epoch is:
//   actualDays = 365*y + leapAdjustment + daysFromMarch1[marchMonth] + (d-1)
// Since we only care about (actualDays % 7), and 365 % 7 == 1, we reduce
//   365*y  →  y   (mod 7)
// The result is no longer a real day count — it is a mod-7 token.
// Leap-day count uses Gregorian rules: every 4y, skip 100y, restore 400y.
// The +3 offset calibrates to the epoch: March 1, year 0 was a Wednesday (index
// 3).
constexpr DayOfWeek dayOfWeek(int y, int m, int d) {
  // Prefix-sum of days from March 1: Mar Apr May Jun Jul Aug Sep Oct Nov Dec
  // Jan Feb
  constexpr std::array<int, 12> daysFromMarch1 = {0,   31,  61,  92,  122, 153,
                                                  184, 214, 245, 275, 306, 337};

  if (m < 3) {
    y--;
  }

  const int marchMonth =
      (m + 9) % 12; // Mar=0, Apr=1, ..., Dec=9, Jan=10, Feb=11
  const int leapYearAdjustment = y / 4 - y / 100 + y / 400;

  // Shortcut: 365*y ≡ y (mod 7) because 365 = 52*7 + 1, so one full year
  // always shifts the day of week forward by exactly 1. We substitute y for
  // 365*y so the value below is NOT a real day count — only its remainder
  // mod 7 is meaningful.
  const int mod7Token =
      y + leapYearAdjustment + daysFromMarch1[marchMonth] + (d - 1);

  return static_cast<DayOfWeek>((mod7Token + 3) % 7);
}

constexpr bool isLeapYear(int year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

constexpr int daysInMonth(int year, int month) {
  constexpr std::array<int, 12> days = {31, 28, 31, 30, 31, 30,
                                        31, 31, 30, 31, 30, 31};
  if (month == 2 && isLeapYear(year)) {
    return 29;
  }
  return days[month - 1];
}

} // namespace date_common_functions
