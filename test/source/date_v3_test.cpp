#include "date_common_functions.hpp"
#include "date_helper.hpp"
#include "date_v3.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

using date_common_functions::DayOfWeek;

// ---- date_helper ----

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

TEST(DateHelper, GetNextWeekday_ThursdayToFriday) {
  EXPECT_EQ(date_helper::getNextWeekday(20260101), 20260102);
  EXPECT_EQ(date_helper::dayOfWeek(20260102), DayOfWeek::Friday);
}

TEST(DateHelper, GetNextWeekday_FridaySkipsToMonday) {
  EXPECT_EQ(date_helper::getNextWeekday(20260102), 20260105);
}

TEST(DateHelper, GetNextWeekday_SaturdaySkipsToMonday) {
  EXPECT_EQ(date_helper::getNextWeekday(20260103), 20260105);
}

TEST(DateHelper, GetNextWeekday_SundaySkipsToMonday) {
  EXPECT_EQ(date_helper::getNextWeekday(20260104), 20260105);
}

TEST(DateHelper, GetNextWeekday_YearRollover) {
  EXPECT_EQ(date_helper::getNextWeekday(20261231), 20270101);
  EXPECT_EQ(date_helper::dayOfWeek(20270101), DayOfWeek::Friday);
}

// ---- serialize ----

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
