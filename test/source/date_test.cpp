// legacy_date_helper
#include "legacy_date_helper.hpp"
#include "date_v1.hpp"
#include "date_v2.hpp"

// gtest
#include <gtest/gtest.h>

using legacy_date_helper::DayOfWeek;
using legacy_date_helper::dayOfWeek;
using legacy_date_helper::daysInMonth2026;
using legacy_date_helper::getNextWeekday;
using legacy_date_helper::nextDay2026;

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

// ---- daysInMonth2026 ----

TEST(DaysInMonth2026, AllMonths) {
    EXPECT_EQ(daysInMonth2026(1),  31);  // Jan
    EXPECT_EQ(daysInMonth2026(2),  28);  // Feb — 2026 is not a leap year
    EXPECT_EQ(daysInMonth2026(3),  31);  // Mar
    EXPECT_EQ(daysInMonth2026(4),  30);  // Apr
    EXPECT_EQ(daysInMonth2026(5),  31);  // May
    EXPECT_EQ(daysInMonth2026(6),  30);  // Jun
    EXPECT_EQ(daysInMonth2026(7),  31);  // Jul
    EXPECT_EQ(daysInMonth2026(8),  31);  // Aug
    EXPECT_EQ(daysInMonth2026(9),  30);  // Sep
    EXPECT_EQ(daysInMonth2026(10), 31);  // Oct
    EXPECT_EQ(daysInMonth2026(11), 30);  // Nov
    EXPECT_EQ(daysInMonth2026(12), 31);  // Dec
}

// ---- nextDay2026 ----

TEST(NextDay2026_V1, MidMonth) {
    date_v1::Date d{2026, 3, 9};
    auto next = nextDay2026(d);
    EXPECT_EQ(next.year,  2026);
    EXPECT_EQ(next.month, 3);
    EXPECT_EQ(next.day,   10);
}

TEST(NextDay2026_V1, MonthRollover) {
    date_v1::Date d{2026, 1, 31};
    auto next = nextDay2026(d);
    EXPECT_EQ(next.year,  2026);
    EXPECT_EQ(next.month, 2);
    EXPECT_EQ(next.day,   1);
}

TEST(NextDay2026_V1, FebruaryEnd) {
    date_v1::Date d{2026, 2, 28};
    auto next = nextDay2026(d);
    EXPECT_EQ(next.year,  2026);
    EXPECT_EQ(next.month, 3);
    EXPECT_EQ(next.day,   1);
}

TEST(NextDay2026_V1, YearRollover) {
    date_v1::Date d{2026, 12, 31};
    auto next = nextDay2026(d);
    EXPECT_EQ(next.year,  2027);
    EXPECT_EQ(next.month, 1);
    EXPECT_EQ(next.day,   1);
}

TEST(NextDay2026_V2, MonthRollover) {
    date_v2::Date d{2026, 1, 31};
    auto next = nextDay2026(d);
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

TEST(GetNextWeekday_V1, YearRollover) {
    date_v1::Date d{2026, 12, 31};  // Thursday
    auto next = getNextWeekday(d);
    EXPECT_EQ(next.year,  2027);
    EXPECT_EQ(next.month, 1);
    EXPECT_EQ(next.day,   1);
    EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Friday);
}

TEST(GetNextWeekday_V2, YearRollover) {
    date_v2::Date d{2026, 12, 31};  // Thursday
    auto next = getNextWeekday(d);
    EXPECT_EQ(next.year,  2027);
    EXPECT_EQ(next.month, 1);
    EXPECT_EQ(next.day,   1);
    EXPECT_EQ(dayOfWeek(next.year, next.month, next.day), DayOfWeek::Friday);
}
