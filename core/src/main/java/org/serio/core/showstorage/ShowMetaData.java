package org.serio.core.showstorage;

import org.apache.commons.lang3.StringUtils;

import java.util.Objects;
import java.util.UUID;

/**
 * Meta-data, that describes a show.
 *
 * <p>Contains only the information about the show itself. Does not include information about the episodes
 * of the show.</p>
 */
public class ShowMetaData {
    private final UUID id;
    private final String name;
    private final String thumbnailUrl;

    /**
     * Create show meta-data.
     *
     * @param id ID of the show. If null - the show's ID will be generated based of the specified name
     * @param name name of the show. Can't be empty or null.
     * @param thumbnailUrl URL to a thumbnail image file, representing this show. If null is specified - the show will
     *                     not have a thumbnail
     */
    public ShowMetaData(UUID id, String name, String thumbnailUrl) {
        if (StringUtils.isEmpty(name)) {
            throw new IllegalArgumentException("Name of the show is not specified or empty");
        }
        this.id = id != null ? id : UUID.nameUUIDFromBytes(name.getBytes());
        this.name = name;
        this.thumbnailUrl = thumbnailUrl != null ? thumbnailUrl : "";
    }

    /**
     * Get ID of the show.
     *
     * @return ID of the show
     */
    public UUID getId() {
        return id;
    }

    /**
     * Get name of the show.
     *
     * @return name of the show
     */
    public String getName() {
        return name;
    }

    /**
     * Get URL of the thumbnail image, representing the show.
     *
     * @return URL to the show's thumbnail. Won't be null even if the show does not have a thumbnail:
     * some placeholder value will be returned.
     */
    public String getThumbnailUrl() {
        return thumbnailUrl;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ShowMetaData that = (ShowMetaData) o;
        return Objects.equals(id, that.id) &&
                Objects.equals(name, that.name) &&
                Objects.equals(thumbnailUrl, that.thumbnailUrl);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(id, name, thumbnailUrl);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "ShowMetaData{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", thumbnailUrl='" + thumbnailUrl + '\'' +
                '}';
    }
}
