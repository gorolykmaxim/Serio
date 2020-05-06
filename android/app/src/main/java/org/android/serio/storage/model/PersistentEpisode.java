package org.android.serio.storage.model;

import androidx.annotation.NonNull;
import androidx.room.ColumnInfo;
import androidx.room.Entity;
import androidx.room.ForeignKey;

import org.serio.core.showstorage.Episode;
import org.serio.core.showstorage.Show;

@Entity(tableName = "EPISODE", primaryKeys = {"ID", "SHOW_ID"}, foreignKeys = {@ForeignKey(onDelete = ForeignKey.CASCADE, entity = PersistentShow.class, parentColumns = "ID", childColumns = "SHOW_ID")})
public class PersistentEpisode {
    @ColumnInfo(name = "ID")
    private int id;
    @ColumnInfo(name = "SHOW_ID")
    @NonNull
    private String showId;
    @ColumnInfo(name = "NAME")
    @NonNull
    private String name;
    @ColumnInfo(name = "VIDEO_URL")
    @NonNull
    private String videoUrl;

    public PersistentEpisode(int id, @NonNull String showId, @NonNull String name, @NonNull String videoUrl) {
        this.id = id;
        this.showId = showId;
        this.name = name;
        this.videoUrl = videoUrl;
    }

    public PersistentEpisode(Show show, Episode episode) {
        id = episode.getId();
        showId = show.getId().toString();
        name = episode.getName();
        videoUrl = episode.getVideoUrl();
    }

    public int getId() {
        return id;
    }

    @NonNull
    public String getShowId() {
        return showId;
    }

    @NonNull
    public String getName() {
        return name;
    }

    @NonNull
    public String getVideoUrl() {
        return videoUrl;
    }

    public Episode toEpisode() {
        return new Episode(id, name, videoUrl);
    }
}
