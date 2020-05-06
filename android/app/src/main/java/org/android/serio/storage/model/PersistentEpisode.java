package org.android.serio.storage.model;

import androidx.annotation.NonNull;
import androidx.room.ColumnInfo;
import androidx.room.Entity;
import androidx.room.ForeignKey;

import org.serio.core.showstorage.Episode;
import org.serio.core.showstorage.Show;

/**
 * Database representation of {@link Episode}.
 */
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

    /**
     * This constructor is used by the Room.
     */
    public PersistentEpisode(int id, @NonNull String showId, @NonNull String name, @NonNull String videoUrl) {
        this.id = id;
        this.showId = showId;
        this.name = name;
        this.videoUrl = videoUrl;
    }

    /**
     * Construct a persistent version of the specified episode and it's show.
     *
     * @param show show to which this episode belongs to
     * @param episode episode to be constructed based of
     */
    public PersistentEpisode(Show show, Episode episode) {
        id = episode.getId();
        showId = show.getId().toString();
        name = episode.getName();
        videoUrl = episode.getVideoUrl();
    }

    /**
     * @see Episode#getId()
     */
    public int getId() {
        return id;
    }

    /**
     * @see Show#getId()
     */
    @NonNull
    public String getShowId() {
        return showId;
    }

    /**
     * @see Episode#getName()
     */
    @NonNull
    public String getName() {
        return name;
    }

    /**
     * @see Episode#getVideoUrl()
     */
    @NonNull
    public String getVideoUrl() {
        return videoUrl;
    }

    /**
     * Convert this episode into it's original representation.
     *
     * @return original {@link Episode}
     */
    public Episode toEpisode() {
        return new Episode(id, name, videoUrl);
    }
}
