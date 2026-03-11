#include "date_common_functions.hpp"

#include <gtest/gtest.h>

using date_common_functions::DayOfWeek;
using date_common_functions::dayOfWeek;
using date_common_functions::daysInMonth;
using date_common_functions::isLeapYear;

// ---- isLeapYear ----

TEST(IsLeapYear, DivisibleBy4) {
  EXPECT_TRUE(isLeapYear(2024));
  EXPECT_FALSE(isLeapYear(2026));
}

TEST(IsLeapYear, DivisibleBy100) {
  EXPECT_FALSE(isLeapYear(1900));
  EXPECT_FALSE(isLeapYear(2100));
}

TEST(IsLeapYear, DivisibleBy400) {
  EXPECT_TRUE(isLeapYear(2000));
  EXPECT_TRUE(isLeapYear(2400));
}

// ---- dayOfWeek ----

TEST(DayOfWeek, KnownWeekdaysIn2026) {
  EXPECT_EQ(dayOfWeek(2026, 1, 1), DayOfWeek::Thursday);
  EXPECT_EQ(dayOfWeek(2026, 1, 2), DayOfWeek::Friday);
  EXPECT_EQ(dayOfWeek(2026, 1, 5), DayOfWeek::Monday);
  EXPECT_EQ(dayOfWeek(2026, 3, 9), DayOfWeek::Monday);
  EXPECT_EQ(dayOfWeek(2026, 12, 31), DayOfWeek::Thursday);
}

TEST(DayOfWeek, WeekendDatesIn2026) {
  EXPECT_EQ(dayOfWeek(2026, 1, 3), DayOfWeek::Saturday);
  EXPECT_EQ(dayOfWeek(2026, 1, 4), DayOfWeek::Sunday);
  EXPECT_EQ(dayOfWeek(2026, 2, 28), DayOfWeek::Saturday);
}

// ---- daysInMonth ----

TEST(DaysInMonth, NonLeapYear) {
  EXPECT_EQ(daysInMonth(2026, 1), 31);
  EXPECT_EQ(daysInMonth(2026, 2), 28);
  EXPECT_EQ(daysInMonth(2026, 3), 31);
  EXPECT_EQ(daysInMonth(2026, 4), 30);
  EXPECT_EQ(daysInMonth(2026, 12), 31);
}

TEST(DaysInMonth, LeapYear) {
  EXPECT_EQ(daysInMonth(2024, 2), 29);
  EXPECT_EQ(daysInMonth(2000, 2), 29);
  EXPECT_EQ(daysInMonth(1900, 2), 28);
}
