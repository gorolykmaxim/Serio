package org.serio.core.showplayer;

import org.serio.core.shows.Shows;
import org.serio.core.shows.WatchableShow;
import org.serio.core.watchhistory.WatchProgress;

import java.util.UUID;

/**
 * Plays episodes of shows according to the watch history.
 *
 * <p>Can play only one show at a time. One of the play*() methods should be called to start playing a show
 * before calling any other methods of this module.</p>
 */
public class ShowPlayer {
    private final Shows shows;
    private volatile Player player;

    /**
     * Construct a show player module.
     *
     * @param shows module to look for shows in and to report playback progress to
     */
    public ShowPlayer(Shows shows) {
        this.shows = shows;
        player = null;
    }

    /**
     * @see Player#of(WatchableShow, boolean)
     */
    public Player playShow(UUID showId, boolean fromBeginning) {
        try  {
            WatchableShow show = shows.findShowById(showId);
            return setPlayer(Player.of(show, fromBeginning));
        } catch (Exception e) {
            throw new PlayShowException(showId, fromBeginning, e);
        }
    }

    /**
     * @see Player#of(WatchableShow, int)
     */
    public Player playShowEpisode(UUID showId, int episodeId) {
        try {
            WatchableShow show = shows.findShowById(showId);
            return setPlayer(Player.of(show, episodeId));
        } catch (Exception e) {
            throw new PlayShowException(showId, episodeId, e);
        }
    }

    /**
     * Report playback progress of the currently playing show episode.
     *
     * @param progress playback progress of the currently playing episode in percents (from 1 to 100)
     */
    public void reportPlaybackProgress(double progress) {
        try {
            Player player = getPlayer();
            UUID showId = player.getPlayingShowId();
            int episodeID = player.getPlayingEpisodeIdOrFail();
            shows.watchShowEpisode(showId, episodeID, WatchProgress.fromPercentage(progress));
        } catch (Exception e) {
            throw new ProgressReportException(progress, e);
        }
    }

    /**
     * Play next episode of the currently playing show.
     *
     * @return current state of the show player
     */
    public Player playNextEpisode() {
        try {
            return setPlayer(getPlayer().nextEpisode());
        } catch (Exception e) {
            throw EpisodeChangeException.next(e);
        }
    }

    /**
     * Play previous episode of the currently playing show.
     *
     * @return current state of the show player
     */
    public Player playPreviousEpisode() {
        try {
            return setPlayer(getPlayer().previousEpisode());
        } catch (Exception e) {
            throw EpisodeChangeException.previous(e);
        }
    }

    private Player setPlayer(Player player) {
        this.player = player;
        return player;
    }

    private Player getPlayer() {
        if (player == null) {
            throw new NothingIsPlayingException();
        }
        return player;
    }
}
