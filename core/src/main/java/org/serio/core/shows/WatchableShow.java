package org.serio.core.shows;

import org.serio.core.showstorage.Show;
import org.serio.core.watchhistory.EpisodeView;
import org.serio.core.watchhistory.ShowView;

import java.time.LocalDate;
import java.util.*;
import java.util.stream.Collectors;

public class WatchableShow {
    private final WatchableShowMetaData metaData;
    private final List<WatchableEpisode> episodes;

    static WatchableShow from(Show show, List<EpisodeView> episodeViews) {
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

    public UUID getId() {
        return metaData.getId();
    }

    public String getName() {
        return metaData.getName();
    }

    public String getThumbnailUrl() {
        return metaData.getThumbnailUrl();
    }

    public boolean hasBeenWatched() {
        return metaData.hasBeenWatched();
    }

    public Optional<LocalDate> getLastWatchedDate() {
        return metaData.getLastWatchedDate();
    }

    public List<WatchableEpisode> getEpisodes() {
        return episodes;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        WatchableShow that = (WatchableShow) o;
        return Objects.equals(metaData, that.metaData) &&
                Objects.equals(episodes, that.episodes);
    }

    @Override
    public int hashCode() {
        return Objects.hash(metaData, episodes);
    }

    @Override
    public String toString() {
        return "WatchableShow{" +
                "metaData=" + metaData +
                ", episodes=" + episodes +
                '}';
    }
}
