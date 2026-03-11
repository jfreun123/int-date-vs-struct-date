#include "date_common_functions.hpp"
#include "date_v1.hpp"
#include "legacy_date_helper.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

using date_common_functions::DayOfWeek;
using date_common_functions::dayOfWeek;
using legacy_date_helper::getNextWeekday;
using legacy_date_helper::nextDay;

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

// ---- getNextWeekday ----

TEST(GetNextWeekday_V1, ThursdayToFriday) {
  date_v1::Date d{2026, 1, 1};
  auto next = getNextWeekday(d);
  EXPECT_EQ(next.day, 2);
  EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Friday);
}

TEST(GetNextWeekday_V1, FridaySkipsToMonday) {
  date_v1::Date d{2026, 1, 2};
  auto next = getNextWeekday(d);
  EXPECT_EQ(next.day, 5);
  EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Monday);
}

TEST(GetNextWeekday_V1, SaturdaySkipsToMonday) {
  date_v1::Date d{2026, 1, 3};
  auto next = getNextWeekday(d);
  EXPECT_EQ(next.day, 5);
  EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Monday);
}

TEST(GetNextWeekday_V1, SundaySkipsToMonday) {
  date_v1::Date d{2026, 1, 4};
  auto next = getNextWeekday(d);
  EXPECT_EQ(next.day, 5);
  EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Monday);
}

TEST(GetNextWeekday_V1, YearRollover) {
  date_v1::Date d{2026, 12, 31};
  auto next = getNextWeekday(d);
  EXPECT_EQ(next.year, 2027);
  EXPECT_EQ(next.month, 1);
  EXPECT_EQ(next.day, 1);
  EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Friday);
}

// ---- serialize ----

TEST(Serialize_V1, RoundTrip) {
  date_v1::Date src{2026, 3, 9};
  int wire = (src.year * 10000) + (src.month * 100) + src.day;
  EXPECT_EQ(wire, 20260309);
  date_v1::Date dst{wire / 10000, (wire / 100) % 100, wire % 100};
  EXPECT_EQ(dst.year, src.year);
  EXPECT_EQ(dst.month, src.month);
  EXPECT_EQ(dst.day, src.day);
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
