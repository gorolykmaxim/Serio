package org.serio.core.showstorage;

import org.apache.commons.lang3.StringUtils;

import java.util.Objects;
import java.util.UUID;

public class ShowMetaData {
    private final UUID id;
    private final String name;
    private final String thumbnailUrl;

    public ShowMetaData(UUID id, String name, String thumbnailUrl) {
        if (StringUtils.isEmpty(name)) {
            throw new IllegalArgumentException("Name of the show is not specified or empty");
        }
        this.id = id != null ? id : UUID.nameUUIDFromBytes(name.getBytes());
        this.name = name;
        this.thumbnailUrl = thumbnailUrl != null ? thumbnailUrl : "";
    }

    public UUID getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public String getThumbnailUrl() {
        return thumbnailUrl;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ShowMetaData that = (ShowMetaData) o;
        return Objects.equals(id, that.id) &&
                Objects.equals(name, that.name) &&
                Objects.equals(thumbnailUrl, that.thumbnailUrl);
    }

    @Override
    public int hashCode() {
        return Objects.hash(id, name, thumbnailUrl);
    }

    @Override
    public String toString() {
        return "ShowMetaData{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", thumbnailUrl='" + thumbnailUrl + '\'' +
                '}';
    }
}
