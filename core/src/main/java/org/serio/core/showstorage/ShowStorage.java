package org.serio.core.showstorage;

import java.util.List;
import java.util.UUID;

public interface ShowStorage {
    Show findById(UUID id);
    List<Show> findAll();
    void save(Show show);
    void deleteById(UUID id);
}
