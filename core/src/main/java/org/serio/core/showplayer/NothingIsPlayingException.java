package org.serio.core.showplayer;

public class NothingIsPlayingException extends IllegalStateException {
    public NothingIsPlayingException() {
        super("Nothing is playing right now");
    }

    public NothingIsPlayingException(Player player) {
        super(String.format("%s is not playing right now", player));
    }
}
