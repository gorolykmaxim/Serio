package org.serio.core.applicationcontroller.event;

import org.serio.core.showplayer.Player;
import org.serio.core.shows.WatchableEpisode;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.Objects;
import java.util.Optional;
import java.util.UUID;

/**
 * Event that will trigger rendering of {@link ViewIds#SHOW_PLAYER} view.
 */
public class ShowPlayerEvent implements ApplicationEvent {
    private final Player player;

    /**
     * Construct an event.
     *
     * @param player information according to which an actual user interface player should be rendered
     */
    public ShowPlayerEvent(Player player) {
        if (player == null) {
            throw new IllegalArgumentException("Player is not specified");
        }
        this.player = player;
    }

    /**
     * @see Player#getPlayingShowId()
     */
    public UUID getShowId() {
        return player.getPlayingShowId();
    }

    /**
     * Get name of the show, to should be played by the player.
     *
     * @return name of the show playing
     */
    public String getShowName() {
        return player.getPlayingShow().getName();
    }

    /**
     * Get name of the episode, that should be played by the player.
     *
     * @return name of the playing episode. Can be empty if the actual player should not be playing.
     */
    public Optional<String> getEpisodeName() {
        return player.getPlayingEpisode().map(WatchableEpisode::getName);
    }

    /**
     * Get video URL of the episode, that should be played by the player.
     *
     * @return video URL of the episode to play. Can be empty if the actual player should not be playing.
     */
    public Optional<String> getVideoUrl() {
        return player.getPlayingEpisode().map(WatchableEpisode::getVideoUrl);
    }

    /**
     * Check if the currently playing episode has a previous episode.
     *
     * @return true if there is a previous episode to play
     */
    public boolean hasPreviousEpisode() {
        return player.hasPreviousEpisode();
    }

    /**
     * Check if the currently playing episode has a next episode.
     *
     * @return true if there is a next episode to play
     */
    public boolean hasNextEpisode() {
        return player.hasNextEpisode();
    }

    /**
     * Get the percentage of the playing video, from which the user interface player should start playing it.
     *
     * @return fraction of the video to start playing from
     */
    public double getStartProgress() {
        return player.getStartProgress();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getViewId() {
        return ViewIds.SHOW_PLAYER;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ShowPlayerEvent that = (ShowPlayerEvent) o;
        return Objects.equals(player, that.player);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(player);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "ShowPlayerEvent{" +
                "player=" + player +
                '}';
    }
}
