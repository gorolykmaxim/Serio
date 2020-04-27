package org.serio.core.applicationcontroller.event;

import org.serio.core.showplayer.Player;
import org.serio.core.shows.WatchableEpisode;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;
import org.serio.core.watchhistory.WatchProgress;

import java.util.Objects;
import java.util.Optional;

public class ShowPlayerEvent implements ApplicationEvent {
    private final Player player;

    public ShowPlayerEvent(Player player) {
        if (player == null) {
            throw new IllegalArgumentException("Player is not specified");
        }
        this.player = player;
    }

    public String getShowName() {
        return player.getPlayingShow().getName();
    }

    public Optional<String> getEpisodeName() {
        return player.getPlayingEpisode().map(WatchableEpisode::getName);
    }

    public Optional<String> getVideoUrl() {
        return player.getPlayingEpisode().map(WatchableEpisode::getVideoUrl);
    }

    public boolean hasPreviousEpisode() {
        return player.hasPreviousEpisode();
    }

    public boolean hasNextEpisode() {
        return player.hasNextEpisode();
    }

    public Optional<Double> getStartProgress() {
        return player.getPlayingEpisode()
                .map(WatchableEpisode::getWatchProgress)
                .map(WatchProgress::getPercentage);
    }

    @Override
    public int getViewId() {
        return ViewIds.SHOW_PLAYER;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ShowPlayerEvent that = (ShowPlayerEvent) o;
        return Objects.equals(player, that.player);
    }

    @Override
    public int hashCode() {
        return Objects.hash(player);
    }

    @Override
    public String toString() {
        return "ShowPlayerEvent{" +
                "player=" + player +
                '}';
    }
}
