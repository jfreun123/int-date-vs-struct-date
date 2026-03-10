#pragma once

#include "date_common_functions.hpp"

namespace legacy_date_helper {

template <typename Date> constexpr Date nextDay(Date date) {
  using YearType = decltype(date.year);
  using MonthType = decltype(date.month);
  using DayType = decltype(date.day);

  if (date.day < date_common_functions::daysInMonth(date.year, date.month)) {
    return {date.year, date.month, static_cast<DayType>(date.day + 1)};
  }
  if (date.month < 12) {
    return {date.year, static_cast<MonthType>(date.month + 1),
            static_cast<DayType>(1)};
  }
  return {static_cast<YearType>(date.year + 1), static_cast<MonthType>(1),
          static_cast<DayType>(1)};
}

template <typename Date> constexpr Date getNextWeekday(Date date) {
  // Compute DOW on today rather than on nextDay(date), avoiding constructing
  // an intermediate Date just to check its day-of-week.
  constexpr int kAdvance[7] = {1, 1, 1, 1, 1, 3, 2}; // Sun=0 … Sat=6
  const int advance = kAdvance[static_cast<int>(
      date_common_functions::dayOfWeek(date.year, date.month, date.day))];

  Date result = date;
  for (int i = 0; i < advance; ++i) {
    result = nextDay(result);
  }
  return result;
}

} // namespace legacy_date_helper
