package org.serio.core.applicationcontroller.event;

import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.Objects;
import java.util.Optional;

/**
 * Event that will trigger rendering of {@link ViewIds#IMPORT_SHOW_CRAWLER} view.
 */
public class ImportShowFromJsonEvent implements ApplicationEvent {
    private final String rawShowCrawler;

    /**
     * @see ImportShowFromJsonEvent#ImportShowFromJsonEvent(String)
     */
    public ImportShowFromJsonEvent() {
        this(null);
    }

    /**
     * Construct an event.
     *
     * @param rawShowCrawler body of the entire show crawler, that should be imported
     */
    public ImportShowFromJsonEvent(String rawShowCrawler) {
        this.rawShowCrawler = rawShowCrawler;
    }

    /**
     * Body of the entire show crawler, that should be imported.
     *
     * @return show crawler body. Will be empty if was not specified during construction.
     */
    public Optional<String> getRawShowCrawler() {
        return Optional.ofNullable(rawShowCrawler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getViewId() {
        return ViewIds.IMPORT_SHOW_CRAWLER;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ImportShowFromJsonEvent event = (ImportShowFromJsonEvent) o;
        return Objects.equals(rawShowCrawler, event.rawShowCrawler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(rawShowCrawler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "ImportShowFromJsonEvent{" +
                "rawShowCrawler='" + rawShowCrawler + '\'' +
                '}';
    }
}
