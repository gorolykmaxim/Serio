package org.android.serio.storage.model;

import java.time.Instant;
import java.time.LocalDateTime;
import java.time.ZoneId;

/**
 * Database representation of {@link LocalDateTime}.
 */
public class PersistentLocalDateTime {
    private long time;

    /**
     * Create persistent representation of the specified local date time.
     *
     * @param localDateTime local date time to be constructed based of
     * @return persistent representation of the specified local date time
     */
    public static PersistentLocalDateTime from(LocalDateTime localDateTime) {
        long milliseconds = localDateTime.atZone(ZoneId.systemDefault()).toInstant().toEpochMilli();
        return new PersistentLocalDateTime(milliseconds);
    }

    /**
     * Create persistent representation of the specified time.
     *
     * @param milliseconds time in milliseconds since epoch
     * @return persistent representation of the specified time
     */
    public static PersistentLocalDateTime from(long milliseconds) {
        return new PersistentLocalDateTime(milliseconds);
    }

    private PersistentLocalDateTime(long time) {
        this.time = time;
    }

    /**
     * Convert to a corresponding local date time.
     *
     * @return corresponding local date time
     */
    public LocalDateTime toLocalDateTime() {
        return Instant.ofEpochMilli(time).atZone(ZoneId.systemDefault()).toLocalDateTime();
    }

    /**
     * Convert to a corresponding representation in milliseconds since epoch.
     *
     * @return corresponding time in milliseconds
     */
    public long toMilliseconds() {
        return time;
    }
}
