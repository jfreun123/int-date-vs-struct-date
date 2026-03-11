#include "date_common_functions.hpp"
#include "date_helper.hpp"
#include "date_v1.hpp"
#include "date_v2.hpp"
#include "date_v3.hpp"
#include "legacy_date_helper.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

using date_common_functions::DayOfWeek;
using date_common_functions::dayOfWeek;
using date_common_functions::daysInMonth;
using date_common_functions::isLeapYear;
using legacy_date_helper::getNextWeekday;
using legacy_date_helper::nextDay;

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
  EXPECT_EQ(daysInMonth(2026, 2), 28); // 2026 is not a leap year
  EXPECT_EQ(daysInMonth(2026, 3), 31);
  EXPECT_EQ(daysInMonth(2026, 4), 30);
  EXPECT_EQ(daysInMonth(2026, 12), 31);
}

TEST(DaysInMonth, LeapYear) {
  EXPECT_EQ(daysInMonth(2024, 2), 29);
  EXPECT_EQ(daysInMonth(2000, 2), 29);
  EXPECT_EQ(daysInMonth(1900, 2), 28); // divisible by 100 but not 400
}

// ---- nextDay ----

TEST(NextDay_V1, MidMonth) {
  date_v1::Date d{2026, 3, 9};
  auto next = nextDay(d);
  EXPECT_EQ(next.year, 2026);
  EXPECT_EQ(next.month, 3);
  EXPECT_EQ(next.day, 10);
}

TEST(NextDay_V1, MonthRollover) {
  date_v1::Date d{2026, 1, 31};
  auto next = nextDay(d);
  EXPECT_EQ(next.year, 2026);
  EXPECT_EQ(next.month, 2);
  EXPECT_EQ(next.day, 1);
}

TEST(NextDay_V1, FebruaryEndNonLeap) {
  date_v1::Date d{2026, 2, 28};
  auto next = nextDay(d);
  EXPECT_EQ(next.year, 2026);
  EXPECT_EQ(next.month, 3);
  EXPECT_EQ(next.day, 1);
}

TEST(NextDay_V1, FebruaryEndLeap) {
  date_v1::Date d{2024, 2, 28};
  auto next = nextDay(d);
  EXPECT_EQ(next.year, 2024);
  EXPECT_EQ(next.month, 2);
  EXPECT_EQ(next.day, 29);
}

TEST(NextDay_V1, YearRollover) {
  date_v1::Date d{2026, 12, 31};
  auto next = nextDay(d);
  EXPECT_EQ(next.year, 2027);
  EXPECT_EQ(next.month, 1);
  EXPECT_EQ(next.day, 1);
}

TEST(NextDay_V2, MonthRollover) {
  date_v2::Date d{2026, 1, 31};
  auto next = nextDay(d);
  EXPECT_EQ(next.year, 2026);
  EXPECT_EQ(next.month, 2);
  EXPECT_EQ(next.day, 1);
}

// ---- getNextWeekday ----

TEST(GetNextWeekday_V1, ThursdayToFriday) {
  date_v1::Date d{2026, 1, 1}; // Thursday
  auto next = getNextWeekday(d);
  EXPECT_EQ(next.day, 2);
  EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Friday);
}

TEST(GetNextWeekday_V1, FridaySkipsToMonday) {
  date_v1::Date d{2026, 1, 2}; // Friday
  auto next = getNextWeekday(d);
  EXPECT_EQ(next.day, 5);
  EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Monday);
}

TEST(GetNextWeekday_V1, SaturdaySkipsToMonday) {
  date_v1::Date d{2026, 1, 3}; // Saturday
  auto next = getNextWeekday(d);
  EXPECT_EQ(next.day, 5);
  EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Monday);
}

TEST(GetNextWeekday_V1, SundaySkipsToMonday) {
  date_v1::Date d{2026, 1, 4}; // Sunday
  auto next = getNextWeekday(d);
  EXPECT_EQ(next.day, 5);
  EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Monday);
}

TEST(GetNextWeekday_V1, YearRollover) {
  date_v1::Date d{2026, 12, 31}; // Thursday
  auto next = getNextWeekday(d);
  EXPECT_EQ(next.year, 2027);
  EXPECT_EQ(next.month, 1);
  EXPECT_EQ(next.day, 1);
  EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Friday);
}

TEST(GetNextWeekday_V2, FridaySkipsToMonday) {
  date_v2::Date d{2026, 1, 2}; // Friday
  auto next = getNextWeekday(d);
  EXPECT_EQ(next.day, 5);
  EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Monday);
}

TEST(GetNextWeekday_V2, SaturdaySkipsToMonday) {
  date_v2::Date d{2026, 1, 3}; // Saturday
  auto next = getNextWeekday(d);
  EXPECT_EQ(next.day, 5);
  EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Monday);
}

TEST(GetNextWeekday_V2, YearRollover) {
  date_v2::Date d{2026, 12, 31}; // Thursday
  auto next = getNextWeekday(d);
  EXPECT_EQ(next.year, 2027);
  EXPECT_EQ(next.month, 1);
  EXPECT_EQ(next.day, 1);
  EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Friday);
}

// ---- date_helper (yyyymmdd int) ----

TEST(DateHelper, Components) {
  constexpr int date = 20260309;
  EXPECT_EQ(date_helper::year(date), 2026);
  EXPECT_EQ(date_helper::month(date), 3);
  EXPECT_EQ(date_helper::day(date), 9);
}

TEST(DateHelper, Pack) {
  EXPECT_EQ(date_helper::pack(2026, 3, 9), 20260309);
  EXPECT_EQ(date_helper::pack(2027, 1, 1), 20270101);
}

TEST(DateHelper, DayOfWeek) {
  EXPECT_EQ(date_helper::dayOfWeek(20260101), DayOfWeek::Thursday);
  EXPECT_EQ(date_helper::dayOfWeek(20260103), DayOfWeek::Saturday);
  EXPECT_EQ(date_helper::dayOfWeek(20260104), DayOfWeek::Sunday);
}

TEST(DateHelper, NextDay_MidMonth) {
  EXPECT_EQ(date_helper::nextDay(20260309), 20260310);
}

TEST(DateHelper, NextDay_MonthRollover) {
  EXPECT_EQ(date_helper::nextDay(20260131), 20260201);
}

TEST(DateHelper, NextDay_FebruaryEndNonLeap) {
  EXPECT_EQ(date_helper::nextDay(20260228), 20260301);
}

TEST(DateHelper, NextDay_FebruaryEndLeap) {
  EXPECT_EQ(date_helper::nextDay(20240228), 20240229);
}

TEST(DateHelper, NextDay_YearRollover) {
  EXPECT_EQ(date_helper::nextDay(20261231), 20270101);
}

TEST(DateHelper, GetNextWeekday_FridaySkipsToMonday) {
  EXPECT_EQ(date_helper::getNextWeekday(20260102), 20260105); // Fri -> Mon
}

TEST(DateHelper, GetNextWeekday_SaturdaySkipsToMonday) {
  EXPECT_EQ(date_helper::getNextWeekday(20260103), 20260105); // Sat -> Mon
}

TEST(DateHelper, GetNextWeekday_SundaySkipsToMonday) {
  EXPECT_EQ(date_helper::getNextWeekday(20260104), 20260105); // Sun -> Mon
}

TEST(DateHelper, GetNextWeekday_ThursdayToFriday) {
  EXPECT_EQ(date_helper::getNextWeekday(20260101), 20260102); // Thu -> Fri
  EXPECT_EQ(date_helper::dayOfWeek(20260102), DayOfWeek::Friday);
}

TEST(DateHelper, GetNextWeekday_YearRollover) {
  EXPECT_EQ(date_helper::getNextWeekday(20261231), 20270101); // Thu -> Fri
  EXPECT_EQ(date_helper::dayOfWeek(20270101), DayOfWeek::Friday);
}

// ---- binary serialization round-trips ----

TEST(Serialize_V1, RoundTrip) {
  date_v1::Date src{2026, 3, 9};
  int wire = (src.year * 10000) + (src.month * 100) + src.day;
  EXPECT_EQ(wire, 20260309);
  date_v1::Date dst{wire / 10000, (wire / 100) % 100, wire % 100};
  EXPECT_EQ(dst.year, src.year);
  EXPECT_EQ(dst.month, src.month);
  EXPECT_EQ(dst.day, src.day);
}

TEST(Serialize_V2, RoundTrip) {
  date_v2::Date src{2026, 3, 9};
  int wire = (src.year * 10000) + (src.month * 100) + src.day;
  EXPECT_EQ(wire, 20260309);
  date_v2::Date dst{static_cast<short>(wire / 10000),
                    static_cast<char>((wire / 100) % 100),
                    static_cast<char>(wire % 100)};
  EXPECT_EQ(dst.year, src.year);
  EXPECT_EQ(dst.month, src.month);
  EXPECT_EQ(dst.day, src.day);
}

TEST(Serialize_V3, RoundTrip) {
  date_v3::Date src = 20260309;
  int wire = 0;
  std::memcpy(&wire, &src, sizeof(src));
  date_v3::Date dst = 0;
  std::memcpy(&dst, &wire, sizeof(wire));
  EXPECT_EQ(dst, src);
}

TEST(Serialize_V3_CharPtr, RoundTrip) {
  date_v3::Date src = 20260309;
  std::array<char, 9> buf{};
  std::snprintf(buf.data(), buf.size(), "%d", src);
  EXPECT_STREQ(buf.data(), "20260309");
  date_v3::Date dst = std::atoi(buf.data());
  EXPECT_EQ(dst, src);
}

// ---- sort order ----

TEST(SortOrder_V1, ShuffledDates) {
  std::vector<date_v1::Date> dates = {
      {2026, 3, 9}, {2026, 1, 1}, {2025, 12, 31}, {2026, 3, 8}, {2026, 1, 31},
  };
  std::ranges::sort(dates);
  EXPECT_EQ((dates[0]), (date_v1::Date{2025, 12, 31}));
  EXPECT_EQ((dates[1]), (date_v1::Date{2026, 1, 1}));
  EXPECT_EQ((dates[2]), (date_v1::Date{2026, 1, 31}));
  EXPECT_EQ((dates[3]), (date_v1::Date{2026, 3, 8}));
  EXPECT_EQ((dates[4]), (date_v1::Date{2026, 3, 9}));
}

TEST(SortOrder_V2, ShuffledDates) {
  std::vector<date_v2::Date> dates = {
      {2026, 3, 9}, {2026, 1, 1}, {2025, 12, 31}, {2026, 3, 8}, {2026, 1, 31},
  };
  std::ranges::sort(dates);
  EXPECT_EQ((dates[0]), (date_v2::Date{2025, 12, 31}));
  EXPECT_EQ((dates[1]), (date_v2::Date{2026, 1, 1}));
  EXPECT_EQ((dates[2]), (date_v2::Date{2026, 1, 31}));
  EXPECT_EQ((dates[3]), (date_v2::Date{2026, 3, 8}));
  EXPECT_EQ((dates[4]), (date_v2::Date{2026, 3, 9}));
}

TEST(SortOrder_V3, ShuffledDates) {
  std::vector<date_v3::Date> dates = {
      20260309, 20260101, 20251231, 20260308, 20260131,
  };
  std::ranges::sort(dates);
  EXPECT_EQ(dates[0], 20251231);
  EXPECT_EQ(dates[1], 20260101);
  EXPECT_EQ(dates[2], 20260131);
  EXPECT_EQ(dates[3], 20260308);
  EXPECT_EQ(dates[4], 20260309);
}
