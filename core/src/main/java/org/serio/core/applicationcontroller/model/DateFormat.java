package org.serio.core.applicationcontroller.model;

import java.time.LocalDate;

public interface DateFormat {
    String applyTo(LocalDate date);
}
