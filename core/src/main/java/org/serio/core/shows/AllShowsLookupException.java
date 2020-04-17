package org.serio.core.shows;

public class AllShowsLookupException extends RuntimeException {
    AllShowsLookupException(Throwable cause) {
        super(String.format("Failed to get a list of all shows. Reason: %s", cause.getMessage()), cause);
    }
}
