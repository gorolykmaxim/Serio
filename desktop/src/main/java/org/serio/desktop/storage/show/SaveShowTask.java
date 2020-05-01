package org.serio.desktop.storage.show;

import org.serio.core.showstorage.Show;
import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;

import java.util.List;
import java.util.Properties;
import java.util.stream.Collectors;

public class SaveShowTask implements StorageTask<Void> {
    private final Show show;
    private final Properties queries;

    public SaveShowTask(Show show, Properties queries) {
        this.show = show;
        this.queries = queries;
    }

    @Override
    public Void execute(JdbcTemplate template) {
        new DeleteShowByIdTask(show.getId(), queries).execute(template);
        template.update(queries.getProperty("create-show"), show.getId().toString(), show.getName(), show.getThumbnailUrl());
        List<Object[]> arguments = show.getEpisodes()
                .stream()
                .map(episode -> new Object[]{episode.getId(), episode.getName(), episode.getVideoUrl()})
                .collect(Collectors.toList());
        template.batchUpdate(queries.getProperty("create-episode"), arguments);
        return null;
    }
}
