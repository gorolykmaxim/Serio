package org.android.serio.storage.model;

import androidx.annotation.NonNull;
import androidx.room.ColumnInfo;
import androidx.room.Entity;

import org.serio.core.showstorage.ShowMetaData;

import java.util.UUID;

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

    public PersistentShow(@NonNull String id, @NonNull String name, @NonNull String thumbnailUrl) {
        this.id = id;
        this.name = name;
        this.thumbnailUrl = thumbnailUrl;
    }

    public PersistentShow(ShowMetaData show) {
        id = show.getId().toString();
        name = show.getName();
        thumbnailUrl = show.getThumbnailUrl();
    }

    @NonNull
    public String getId() {
        return id;
    }

    @NonNull
    public String getName() {
        return name;
    }

    @NonNull
    public String getThumbnailUrl() {
        return thumbnailUrl;
    }

    public ShowMetaData toShowMetaData() {
        return new ShowMetaData(UUID.fromString(id), name, thumbnailUrl);
    }
}
