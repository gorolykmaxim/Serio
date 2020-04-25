package org.serio.core.showplayer;

public class EpisodeChangeException extends ShowPlayerException {
    public static EpisodeChangeException next(Throwable cause) {
        return new EpisodeChangeException("next", cause);
    }

    public static EpisodeChangeException previous(Throwable cause) {
        return new EpisodeChangeException("previous", cause);
    }

    private EpisodeChangeException(String changeType, Throwable cause) {
        super(String.format("play %s episode of the currently playing show", changeType), cause);
    }
}
