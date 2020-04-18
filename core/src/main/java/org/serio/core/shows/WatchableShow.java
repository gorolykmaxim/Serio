package org.serio.core.shows;

import org.serio.core.showstorage.Show;
import org.serio.core.watchhistory.EpisodeView;
import org.serio.core.watchhistory.ShowView;

import java.time.LocalDate;
import java.util.*;
import java.util.stream.Collectors;

/**
 * A show that the user might be watching or might have watched.
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
                .map(episode -> new WatchableEpisode(episode, episodeIdToView.get(Long.toString(episode.getId()))))
                .collect(Collectors.toList());
        Optional<LocalDate> possibleLastWatchDate = episodeViews
                .stream()
                .map(EpisodeView::getLastWatchDate)
                .max(LocalDate::compareTo);
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
     * Get ID of the show.
     *
     * @return ID of the show
     */
    public UUID getId() {
        return metaData.getId();
    }

    /**
     * Get name of the show.
     *
     * @return name of the show
     */
    public String getName() {
        return metaData.getName();
    }

    /**
     * Get URL to the thumbnail image file, that represents the show.
     *
     * @return URL to the thumbnail file
     */
    public String getThumbnailUrl() {
        return metaData.getThumbnailUrl();
    }

    /**
     * Check if this show has been watched by the user.
     *
     * @return true if the show has been watched by the user
     */
    public boolean hasBeenWatched() {
        return metaData.hasBeenWatched();
    }

    /**
     * Get the last date the user has watched one of the episodes of the show
     *
     * @return the last date the user has watched the show
     */
    public Optional<LocalDate> getLastWatchedDate() {
        return metaData.getLastWatchedDate();
    }

    /**
     * Get all episodes of the show.
     *
     * @return collection of episodes of the show
     */
    public List<WatchableEpisode> getEpisodes() {
        return episodes;
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
