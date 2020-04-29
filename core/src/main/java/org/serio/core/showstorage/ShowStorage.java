package org.serio.core.showstorage;

import java.util.List;
import java.util.UUID;
import java.util.concurrent.Future;

/**
 * Storage to store shows and their episodes.
 */
public interface ShowStorage {
    /**
     * Find a show by it's ID.
     *
     * @param id ID of the show
     * @return show with the specified ID. Can't be null. If the show does not exist - an exception will be thrown.
     */
    Future<Show> findById(UUID id);

    /**
     * Find all shows, stored in the storage.
     *
     * @return all shows, stored in the storage or an empty list if there are no shows
     */
    Future<List<ShowMetaData>> findAll();

    /**
     * Check if a show with the specified name is stored in the storage.
     *
     * @param name name of the show to check
     * @return true if there is a show with such a name
     */
    Future<Boolean> containsShowWithName(String name);

    /**
     * Save the specified show in the storage.
     *
     * <p>If a show with the same ID is already being stored in the storage - it will be replaced
     * with the specified one.</p>
     *
     * @param show show to save
     */
    void save(Show show);

    /**
     * Remove a show with the specified ID from the storage.
     *
     * @param id ID of the show to remove
     */
    void deleteById(UUID id);
}
