package org.serio.core.applicationcontroller.event;

import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.Objects;
import java.util.Optional;

public class ImportShowFromJsonEvent implements ApplicationEvent {
    private final String rawShowCrawler;

    public ImportShowFromJsonEvent() {
        this(null);
    }

    public ImportShowFromJsonEvent(String rawShowCrawler) {
        this.rawShowCrawler = rawShowCrawler;
    }

    public Optional<String> getRawShowCrawler() {
        return Optional.ofNullable(rawShowCrawler);
    }

    @Override
    public int getViewId() {
        return ViewIds.IMPORT_SHOW_CRAWLER;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ImportShowFromJsonEvent event = (ImportShowFromJsonEvent) o;
        return Objects.equals(rawShowCrawler, event.rawShowCrawler);
    }

    @Override
    public int hashCode() {
        return Objects.hash(rawShowCrawler);
    }

    @Override
    public String toString() {
        return "ImportShowFromJsonEvent{" +
                "rawShowCrawler='" + rawShowCrawler + '\'' +
                '}';
    }
}
