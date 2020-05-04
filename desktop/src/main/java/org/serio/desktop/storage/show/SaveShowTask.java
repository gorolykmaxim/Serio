package org.serio.desktop.storage.show;

import org.serio.core.showstorage.Show;
import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;

import java.util.List;
import java.util.Properties;
import java.util.stream.Collectors;

/**
 * Save the specified show and it's episodes in the storage.
 *
 * <p>If there is an existing show with the specified ID, both it and all of it's existing episodes will be
 * completely replaced by the specified new one.</p>
 */
public class SaveShowTask implements StorageTask<Void> {
    private final Show show;
    private final Properties queries;

    /**
     * Construct a task.
     *
     * @param show show to save
     * @param queries {@link Properties}, that contains the SQL queries, used by this task
     */
    public SaveShowTask(Show show, Properties queries) {
        this.show = show;
        this.queries = queries;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Void execute(JdbcTemplate template) {
        new DeleteShowByIdTask(show.getId(), queries).execute(template);
        String showId = show.getId().toString();
        template.update(queries.getProperty("create-show"), showId, show.getName(), show.getThumbnailUrl());
        List<Object[]> arguments = show.getEpisodes()
                .stream()
                .map(episode -> new Object[]{episode.getId(), showId, episode.getName(), episode.getVideoUrl()})
                .collect(Collectors.toList());
        template.batchUpdate(queries.getProperty("create-episode"), arguments);
        return null;
    }
}
