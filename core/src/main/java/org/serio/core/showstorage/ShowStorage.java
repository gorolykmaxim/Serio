package org.serio.core.showstorage;

import java.util.List;
import java.util.UUID;
import java.util.concurrent.Future;

public interface ShowStorage {
    Future<Show> findById(UUID id);
    Future<List<ShowMetaData>> findAll();
    void save(Show show);
    void deleteById(UUID id);
}
