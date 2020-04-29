package org.serio.core.showplayer;

import org.serio.core.shows.WatchableEpisode;
import org.serio.core.shows.WatchableShow;

import java.util.Objects;
import java.util.Optional;
import java.util.UUID;

/**
 * Player of an individual show.
 *
 * <p>Can be in one of the two states: playing a show and not playing at all. While being in the second
 * state, the player does not allow any transitions of it's state. Such player state can be considered an end-state
 * since in order to do anything there a new player instance should be created.</p>
 */
public class Player {
    private final WatchableShow show;
    private final Optional<WatchableEpisode> currentEpisode;

    /**
     * Create player of the specified show.
     *
     * <p>By default, the player will start playing the specified show from where the user has left it: if
     * the user has stopped watching the show on the episode #5 - the player will start from that
     * exact episode from the timestamp where the user has finished watching it the last time. In case the
     * user has completely watched the episode #5 - the player will start from the episode #6 instead.</p>
     *
     * <p>If the specified show has not been watched by the user yet - the player will start playing the show
     * from the beginning.</p>
     *
     * @param show tv show to play
     * @param fromBeginning if set to true - forcefully play the show from the beginning, ignoring any information
     *                      about which episodes were already watched by the user
     * @return player of the specified show
     */
    public static Player of(WatchableShow show, boolean fromBeginning) {
        if (fromBeginning) {
            return new Player(show, show.getFirstEpisode());
        } else {
            Optional<WatchableEpisode> lastWatchedEpisode = show.getLastWatchedEpisode();
            Optional<WatchableEpisode> episodeToWatch = lastWatchedEpisode
                    .flatMap(episode -> episode.hasBeenWatchedCompletely() ? show.getEpisodeAfterEpisode(episode.getId()) : Optional.of(episode));
            // If the last watched episode is present - then the show has been already watched and
            // we will play either that episode or the episode after it.
            // Otherwise - the shows has not been watched yet, so we will play it from the beginning.
            return new Player(show, lastWatchedEpisode.isPresent() ? episodeToWatch : show.getFirstEpisode());
        }
    }

    /**
     * Create player of the specified show, that will start playing the show from the specified episode.
     *
     * <p>No watch history information is taken into a consideration by this constructor.</p>
     *
     * @param show show to play
     * @param episodeId episode of the show play
     * @return player of the specified show
     */
    public static Player of(WatchableShow show, int episodeId) {
        return new Player(show, show.getEpisodeById(episodeId));
    }

    private Player(WatchableShow show, Optional<WatchableEpisode> currentEpisode) {
        if (show == null) {
            throw new IllegalArgumentException("Show to be played is not specified");
        }
        this.show = show;
        this.currentEpisode = currentEpisode;
    }

    /**
     * Get ID of the show, being played by the player.
     *
     * @return ID of the show
     */
    public UUID getPlayingShowId() {
        return show.getId();
    }

    /**
     * Get the show, being played by the player.
     *
     * @return show being played
     */
    public WatchableShow getPlayingShow() {
        return show;
    }

    /**
     * Get ID of the episode, being played by the player right now.
     *
     * @return ID of the episode, being played. Can be empty if the player is in the not playing state.
     */
    public Optional<Integer> getPlayingEpisodeId() {
        return currentEpisode.map(WatchableEpisode::getId);
    }

    /**
     * Get the episode, that is being played by the player right now.
     *
     * @return episode, being played. Can be empty if the player is in the not playing state.
     */
    public Optional<WatchableEpisode> getPlayingEpisode() {
        return currentEpisode;
    }

    /**
     * @see Player#getPlayingEpisodeId()
     *
     * Will throw an exception if the player is in the not playing state.
     */
    public int getPlayingEpisodeIdOrFail() {
        return getPlayingEpisodeId().orElseThrow(() -> new NothingIsPlayingException(this));
    }

    /**
     * @see Player#getPlayingEpisode()
     *
     * Will throw an exception if the player is in the not playing state.
     */
    public WatchableEpisode getPlayingEpisodeOrFail() {
        return getPlayingEpisode().orElseThrow(() -> new NothingIsPlayingException(this));
    }

    /**
     * Check if the player is in the playing state or not.
     *
     * @return true if the player is in the playing state and is currently playing some show episode
     */
    public boolean isPlaying() {
        return currentEpisode.isPresent();
    }

    /**
     * Check if the player will be able to play the next episode of it's show
     *
     * @return true if there is a next episode to play in the player's queue
     */
    public boolean hasNextEpisode() {
        return getPlayingEpisodeId()
                .flatMap(show::getEpisodeAfterEpisode)
                .isPresent();
    }

    /**
     * Check if the player will be able to play the previous episode of it's show
     *
     * @return true if there is a previous episode to play in the player's queue
     */
    public boolean hasPreviousEpisode() {
        return getPlayingEpisodeId()
                .flatMap(show::getEpisodeBeforeEpisode)
                .isPresent();
    }

    /**
     * Play the next episode of the current show.
     *
     * @return new instance of the player playing the next episode
     */
    public Player nextEpisode() {
        return new Player(show, show.getEpisodeAfterEpisode(getPlayingEpisodeIdOrFail()));
    }

    /**
     * Play the previous episode of the current show.
     *
     * @return new instance of the player playing the previous episode
     */
    public Player previousEpisode() {
        return new Player(show, show.getEpisodeBeforeEpisode(getPlayingEpisodeIdOrFail()));
    }

    /**
     * Start playing current show from the first episode.
     *
     * @return new instance of the player playing the show from start
     */
    public Player restartShow() {
        return Player.of(show, true);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Player player = (Player) o;
        return Objects.equals(show, player.show) &&
                Objects.equals(currentEpisode, player.currentEpisode);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(show, currentEpisode);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "Player{" +
                "show=" + show +
                ", currentEpisode=" + currentEpisode +
                '}';
    }
}
