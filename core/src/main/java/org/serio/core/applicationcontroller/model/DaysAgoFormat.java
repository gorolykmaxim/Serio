package org.serio.core.applicationcontroller.model;

import java.time.LocalDate;
import java.time.temporal.ChronoUnit;

/**
 * Date format that displays a date as an amount of days between the current date and the the specified one.
 *
 * <p>When formatting a date that corresponds to this day "Today" will be returned. The date that represents
 * yesterday will be formatted as "Yesterday". All other dates will be formatted as "%d days ago": "5 days ago"
 * "11 days ago" etc.</p>
 *
 * <p>Does not work with future dates.</p>
 */
public class DaysAgoFormat implements DateFormat {
    /**
     * {@inheritDoc}
     */
    @Override
    public String applyTo(LocalDate date) {
        long daysAgo = ChronoUnit.DAYS.between(date, LocalDate.now());
        if (daysAgo == 0) {
            return "Today";
        } else if (daysAgo == 1) {
            return "Yesterday";
        } else {
            return daysAgo + " days ago";
        }
    }
}
