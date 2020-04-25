package org.serio.core.showplayer;

import java.util.UUID;

public class PlayShowException extends ShowPlayerException {
    public PlayShowException(UUID showId, boolean fromBeginning, Throwable cause) {
        super(String.format("play show with ID '%s' from the beginning - %s", showId, fromBeginning), cause);
    }

    public PlayShowException(UUID showId, int episodeId, Throwable cause) {
        super(String.format("play episode '%d' of the show with ID '%s'", episodeId, showId), cause);
    }
}
