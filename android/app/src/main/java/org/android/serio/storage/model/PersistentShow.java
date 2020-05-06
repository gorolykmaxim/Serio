package org.android.serio.storage.model;

import androidx.annotation.NonNull;
import androidx.room.ColumnInfo;
import androidx.room.Entity;

import org.serio.core.showstorage.ShowMetaData;

import java.util.UUID;

/**
 * Database representation of {@link ShowMetaData}.
 */
@Entity(tableName = "SHOW", primaryKeys = "ID")
public class PersistentShow {
    @ColumnInfo(name = "ID")
    @NonNull
    private String id;
    @ColumnInfo(name = "NAME")
    @NonNull
    private String name;
    @ColumnInfo(name = "THUMBNAIL_URL")
    @NonNull
    private String thumbnailUrl;

    /**
     * This constructor is used by the Room.
     */
    public PersistentShow(@NonNull String id, @NonNull String name, @NonNull String thumbnailUrl) {
        this.id = id;
        this.name = name;
        this.thumbnailUrl = thumbnailUrl;
    }

    /**
     * Construct a persistent version of the specified show.
     *
     * @param show show to be constructed based of
     */
    public PersistentShow(ShowMetaData show) {
        id = show.getId().toString();
        name = show.getName();
        thumbnailUrl = show.getThumbnailUrl();
    }

    /**
     * @see ShowMetaData#getId()
     */
    @NonNull
    public String getId() {
        return id;
    }

    /**
     * @see ShowMetaData#getName()
     */
    @NonNull
    public String getName() {
        return name;
    }

    /**
     * @see ShowMetaData#getThumbnailUrl()
     */
    @NonNull
    public String getThumbnailUrl() {
        return thumbnailUrl;
    }

    /**
     * Convert this show into it's original version.
     *
     * @return original {@link ShowMetaData}
     */
    public ShowMetaData toShowMetaData() {
        return new ShowMetaData(UUID.fromString(id), name, thumbnailUrl);
    }
}
