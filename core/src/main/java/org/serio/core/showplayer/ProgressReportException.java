package org.serio.core.showplayer;

public class ProgressReportException extends ShowPlayerException {
    public ProgressReportException(double progress, Throwable cause) {
        super(String.format("report new progress (%f) of the currently playing episode", progress), cause);
    }
}
