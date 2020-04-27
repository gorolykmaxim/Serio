package org.serio.core.applicationcontroller.model;

import java.time.LocalDate;
import java.time.temporal.ChronoUnit;

public class DaysAgoFormat implements DateFormat {
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
