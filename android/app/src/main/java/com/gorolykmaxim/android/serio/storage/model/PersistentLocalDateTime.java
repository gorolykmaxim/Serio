package com.gorolykmaxim.android.serio.storage.model;

import java.time.Instant;
import java.time.LocalDateTime;
import java.time.ZoneId;

public class PersistentLocalDateTime {
    private long time;

    public static PersistentLocalDateTime from(LocalDateTime localDateTime) {
        long milliseconds = localDateTime.atZone(ZoneId.systemDefault()).toInstant().toEpochMilli();
        return new PersistentLocalDateTime(milliseconds);
    }

    public static PersistentLocalDateTime from(long milliseconds) {
        return new PersistentLocalDateTime(milliseconds);
    }

    private PersistentLocalDateTime(long time) {
        this.time = time;
    }

    public LocalDateTime toLocalDateTime() {
        return Instant.ofEpochMilli(time).atZone(ZoneId.systemDefault()).toLocalDateTime();
    }

    public long toMilliseconds() {
        return time;
    }
}
