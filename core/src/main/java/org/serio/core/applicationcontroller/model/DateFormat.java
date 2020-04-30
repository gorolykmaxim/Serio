package org.serio.core.applicationcontroller.model;

import java.time.LocalDate;

/**
 * Format of representing a date as a string.
 */
public interface DateFormat {
    /**
     * Apply this format to the specified date and return the result.
     *
     * @param date date to format
     * @return formatted string representation of the date
     */
    String applyTo(LocalDate date);
}
