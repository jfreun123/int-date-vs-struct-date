// legacy_date_helper
#include "legacy_date_helper.hpp"
#include "date_v1.hpp"
#include "date_v2.hpp"

// gtest
#include <gtest/gtest.h>

using legacy_date_helper::DayOfWeek;
using legacy_date_helper::dayOfWeek;
using legacy_date_helper::daysInMonth;
using legacy_date_helper::getNextWeekday;
using legacy_date_helper::isLeapYear;
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
    EXPECT_EQ(daysInMonth(2026, 1),  31);
    EXPECT_EQ(daysInMonth(2026, 2),  28);  // 2026 is not a leap year
    EXPECT_EQ(daysInMonth(2026, 3),  31);
    EXPECT_EQ(daysInMonth(2026, 4),  30);
    EXPECT_EQ(daysInMonth(2026, 12), 31);
}

TEST(DaysInMonth, LeapYear) {
    EXPECT_EQ(daysInMonth(2024, 2), 29);
    EXPECT_EQ(daysInMonth(2000, 2), 29);
    EXPECT_EQ(daysInMonth(1900, 2), 28);  // divisible by 100 but not 400
}

// ---- nextDay ----

TEST(NextDay_V1, MidMonth) {
    date_v1::Date d{2026, 3, 9};
    auto next = nextDay(d);
    EXPECT_EQ(next.year,  2026);
    EXPECT_EQ(next.month, 3);
    EXPECT_EQ(next.day,   10);
}

TEST(NextDay_V1, MonthRollover) {
    date_v1::Date d{2026, 1, 31};
    auto next = nextDay(d);
    EXPECT_EQ(next.year,  2026);
    EXPECT_EQ(next.month, 2);
    EXPECT_EQ(next.day,   1);
}

TEST(NextDay_V1, FebruaryEndNonLeap) {
    date_v1::Date d{2026, 2, 28};
    auto next = nextDay(d);
    EXPECT_EQ(next.year,  2026);
    EXPECT_EQ(next.month, 3);
    EXPECT_EQ(next.day,   1);
}

TEST(NextDay_V1, FebruaryEndLeap) {
    date_v1::Date d{2024, 2, 28};
    auto next = nextDay(d);
    EXPECT_EQ(next.year,  2024);
    EXPECT_EQ(next.month, 2);
    EXPECT_EQ(next.day,   29);
}

TEST(NextDay_V1, YearRollover) {
    date_v1::Date d{2026, 12, 31};
    auto next = nextDay(d);
    EXPECT_EQ(next.year,  2027);
    EXPECT_EQ(next.month, 1);
    EXPECT_EQ(next.day,   1);
}

TEST(NextDay_V2, MonthRollover) {
    date_v2::Date d{2026, 1, 31};
    auto next = nextDay(d);
    EXPECT_EQ(next.year,  2026);
    EXPECT_EQ(next.month, 2);
    EXPECT_EQ(next.day,   1);
}

// ---- getNextWeekday ----

TEST(GetNextWeekday_V1, ThursdayToFriday) {
    date_v1::Date d{2026, 1, 1};  // Thursday
    auto next = getNextWeekday(d);
    EXPECT_EQ(next.day, 2);
    EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Friday);
}

TEST(GetNextWeekday_V1, FridaySkipsToMonday) {
    date_v1::Date d{2026, 1, 2};  // Friday
    auto next = getNextWeekday(d);
    EXPECT_EQ(next.day, 5);
    EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Monday);
}

TEST(GetNextWeekday_V1, SaturdaySkipsToMonday) {
    date_v1::Date d{2026, 1, 3};  // Saturday
    auto next = getNextWeekday(d);
    EXPECT_EQ(next.day, 5);
    EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Monday);
}

TEST(GetNextWeekday_V1, SundaySkipsToMonday) {
    date_v1::Date d{2026, 1, 4};  // Sunday
    auto next = getNextWeekday(d);
    EXPECT_EQ(next.day, 5);
    EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Monday);
}

TEST(GetNextWeekday_V1, YearRollover) {
    date_v1::Date d{2026, 12, 31};  // Thursday
    auto next = getNextWeekday(d);
    EXPECT_EQ(next.year,  2027);
    EXPECT_EQ(next.month, 1);
    EXPECT_EQ(next.day,   1);
    EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Friday);
}

TEST(GetNextWeekday_V2, FridaySkipsToMonday) {
    date_v2::Date d{2026, 1, 2};  // Friday
    auto next = getNextWeekday(d);
    EXPECT_EQ(next.day, 5);
    EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Monday);
}

TEST(GetNextWeekday_V2, SaturdaySkipsToMonday) {
    date_v2::Date d{2026, 1, 3};  // Saturday
    auto next = getNextWeekday(d);
    EXPECT_EQ(next.day, 5);
    EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Monday);
}

TEST(GetNextWeekday_V2, YearRollover) {
    date_v2::Date d{2026, 12, 31};  // Thursday
    auto next = getNextWeekday(d);
    EXPECT_EQ(next.year,  2027);
    EXPECT_EQ(next.month, 1);
    EXPECT_EQ(next.day,   1);
    EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Friday);
}
