package org.serio.core.shows;

import org.serio.core.showstorage.Episode;
import org.serio.core.showstorage.Show;
import org.serio.core.watchhistory.EpisodeView;
import org.serio.core.watchhistory.ShowView;

import java.time.LocalDateTime;
import java.util.*;
import java.util.stream.Collectors;

/**
 * A show that the user might be watching or might have watched.
 *
 * @see Show
 */
public class WatchableShow {
    private final WatchableShowMetaData metaData;
    private final List<WatchableEpisode> episodes;

    /**
     * Combine specified general information about the show and the information about it's episodes, being watched
     * and create an instance of a {@link WatchableShow}.
     *
     * @param show the show to be processed
     * @param episodeViews list of episodes of the specified show, that have been watched by the user
     * @return {@link WatchableShow} representation of the specified show
     */
    public static WatchableShow from(Show show, List<EpisodeView> episodeViews) {
        Map<String, EpisodeView> episodeIdToView = new HashMap<>();
        episodeViews.forEach(episodeView -> episodeIdToView.put(episodeView.getEpisodeId(), episodeView));
        List<WatchableEpisode> episodes = show
                .getEpisodes()
                .stream()
                .sorted(Comparator.comparing(Episode::getId))
                .map(episode -> new WatchableEpisode(episode, episodeIdToView.get(Integer.toString(episode.getId()))))
                .collect(Collectors.toList());
        Optional<LocalDateTime> possibleLastWatchDate = episodeViews
                .stream()
                .map(EpisodeView::getLastWatchDate)
                .max(LocalDateTime::compareTo);
        WatchableShowMetaData metaData;
        metaData = possibleLastWatchDate
                .map(localDate -> new WatchableShowMetaData(show.getMetaData(), new ShowView(show.getId().toString(), localDate)))
                .orElseGet(() -> new WatchableShowMetaData(show.getMetaData(), null));
        return new WatchableShow(metaData, episodes);
    }

    private WatchableShow(WatchableShowMetaData metaData, List<WatchableEpisode> episodes) {
        this.metaData = metaData;
        this.episodes = Collections.unmodifiableList(episodes);
    }

    /**
     * @see Show#getId()
     */
    public UUID getId() {
        return metaData.getId();
    }

    /**
     * @see Show#getName()
     */
    public String getName() {
        return metaData.getName();
    }

    /**
     * @see Show#getThumbnailUrl()
     */
    public String getThumbnailUrl() {
        return metaData.getThumbnailUrl();
    }

    /**
     * @see WatchableShowMetaData#hasBeenWatched()
     */
    public boolean hasBeenWatched() {
        return metaData.hasBeenWatched();
    }

    /**
     * @see Show#getMetaData()
     */
    public WatchableShowMetaData getMetaData() {
        return metaData;
    }

    /**
     * @see WatchableShowMetaData#getLastWatchedDate()
     */
    public Optional<LocalDateTime> getLastWatchedDate() {
        return metaData.getLastWatchedDate();
    }

    /**
     * Returns episodes of this show in their natural order.
     *
     * @see Show#getEpisodes()
     */
    public List<WatchableEpisode> getEpisodes() {
        return episodes;
    }

    /**
     * Get episode of the show with the specified ID.
     *
     * @param episodeId ID of the episode
     * @return episode with the specified ID. Can be empty if the show does not have such episode.
     */
    public Optional<WatchableEpisode> getEpisodeById(int episodeId) {
        if (episodeId < 1 || episodeId > episodes.size()) {
            return Optional.empty();
        } else {
            return Optional.of(episodes.get(episodeId - 1));
        }
    }

    /**
     * Get first episode of this show.
     *
     * @return first episode of the show. Can be empty if the show has no episodes.
     */
    public Optional<WatchableEpisode> getFirstEpisode() {
        return getEpisodeById(1);
    }

    /**
     * Get the previous episode relative to the specified one.
     *
     * @param episodeId ID of the episode
     * @return previous episode relative to the specified one. Can be empty if the specified episode is the first one
     * or does not exist.
     */
    public Optional<WatchableEpisode> getEpisodeBeforeEpisode(int episodeId) {
        return getEpisodeById(episodeId - 1);
    }

    /**
     * Get the next episode relative to the specified one.
     *
     * @param episodeId ID of the episode
     * @return next episode relative to the specified one. Can be empty if the specified episode is the last one
     * or does not exist.
     */
    public Optional<WatchableEpisode> getEpisodeAfterEpisode(int episodeId) {
        return getEpisodeById(episodeId + 1);
    }

    /**
     * Get the episode of this show the user has watched the most recently.
     *
     * @return the most recently watched episode of the show. Will be empty if the user hasn't watched the show yet.
     */
    public Optional<WatchableEpisode> getLastWatchedEpisode() {
        if (!hasBeenWatched()) {
            return Optional.empty();
        }
        return episodes
                .stream()
                .filter(WatchableEpisode::hasBeenWatched)
                .max(Comparator.comparing(episode -> episode.getLastWatchDate().get()));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        WatchableShow that = (WatchableShow) o;
        return Objects.equals(metaData, that.metaData) &&
                Objects.equals(episodes, that.episodes);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(metaData, episodes);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "WatchableShow{" +
                "metaData=" + metaData +
                ", episodes=" + episodes +
                '}';
    }
}
