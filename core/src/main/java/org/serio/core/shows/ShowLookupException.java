package org.serio.core.shows;

import java.util.UUID;

public class ShowLookupException extends RuntimeException {
    ShowLookupException(UUID showId, Throwable cause) {
        super(String.format("Failed to find a show with ID '%s'. Reason: %s", showId, cause.getMessage()), cause);
    }
    ShowLookupException(String showName, Throwable cause) {
        super(String.format("Failed to find a show with name '%s'. Reason: %s", showName, cause.getMessage()), cause);
    }
}
