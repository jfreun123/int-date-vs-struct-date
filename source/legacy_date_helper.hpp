#pragma once

#include "date_common_functions.hpp"

namespace legacy_date_helper {

template <typename Date> constexpr Date nextDay(Date date) {
  if (date.day < date_common_functions::daysInMonth(date.year, date.month)) {
    return {date.year, date.month,
            static_cast<decltype(date.day)>(date.day + 1)};
  }
  if (date.month < 12) {
    return {date.year, static_cast<decltype(date.month)>(date.month + 1),
            static_cast<decltype(date.day)>(1)};
  }
  return {static_cast<decltype(date.year)>(date.year + 1),
          static_cast<decltype(date.month)>(1),
          static_cast<decltype(date.day)>(1)};
}

template <typename Date> constexpr Date getNextWeekday(Date date) {
  Date next = nextDay(date);
  date_common_functions::DayOfWeek dow =
      date_common_functions::dayOfWeek(next.year, next.month, next.day);
  if (dow == date_common_functions::DayOfWeek::Sunday) {
    return nextDay(next);
  }
  if (dow == date_common_functions::DayOfWeek::Saturday) {
    return nextDay(nextDay(next));
  }
  return next;
}

} // namespace legacy_date_helper
