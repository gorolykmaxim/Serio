package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowDetailsEvent;
import org.serio.core.applicationcontroller.model.DisplayableShow;
import org.serio.core.clipboard.Clipboard;
import org.serio.core.notifications.Notifications;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.userinterface.UserInterface;

public class ShareShowCrawlerTask implements ControllerTask {
    private final Notifications notifications;
    private final Clipboard clipboard;
    private final ShowsCrawler showsCrawler;

    public ShareShowCrawlerTask(Notifications notifications, Clipboard clipboard, ShowsCrawler showsCrawler) {
        this.notifications = notifications;
        this.clipboard = clipboard;
        this.showsCrawler = showsCrawler;
    }

    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.peek(ShowDetailsEvent.class).ifPresent(lastEvent -> {
            DisplayableShow show = lastEvent.getShow();
            String serializedShowCrawler = showsCrawler.getSerializedCrawlerOfShow(show.getId().toString());
            clipboard.setContent(serializedShowCrawler);
            notifications.displayTextAsNotification("Crawler is copied to your clipboard");
        });
    }
}
