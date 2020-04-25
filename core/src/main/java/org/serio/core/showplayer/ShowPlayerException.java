package org.serio.core.showplayer;

public class ShowPlayerException extends RuntimeException {
    public ShowPlayerException(String action, Throwable cause) {
        super(String.format("Failed to %s. Reason: %s", action, cause.getMessage()), cause);
    }
}
