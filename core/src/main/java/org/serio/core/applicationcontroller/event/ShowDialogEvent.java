package org.serio.core.applicationcontroller.event;

import org.apache.commons.lang3.StringUtils;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.Objects;

/**
 * Event that triggers rendering of dialog views, related to a specific tv show.
 */
public class ShowDialogEvent implements ApplicationEvent {
    private final int viewId;
    private final String showName;

    /**
     * Will render {@link ViewIds#SHOW_CLEAR_WATCH_HISTORY_DIALOG} view.
     *
     * @see ShowDialogEvent#ShowDialogEvent(int, String)
     */
    public static ShowDialogEvent clearWatchHistoryDialog(String showName) {
        return new ShowDialogEvent(ViewIds.SHOW_CLEAR_WATCH_HISTORY_DIALOG, showName);
    }

    /**
     * Will render {@link ViewIds#SHOW_DELETE_SHOW_DIALOG} view.
     *
     * @see ShowDialogEvent#ShowDialogEvent(int, String)
     */
    public static ShowDialogEvent deleteShowDialog(String showName) {
        return new ShowDialogEvent(ViewIds.SHOW_DELETE_SHOW_DIALOG, showName);
    }

    /**
     * Will render {@link ViewIds#SHOW_WATCH_IS_OVER_DIALOG} view.
     *
     * @see ShowDialogEvent#ShowDialogEvent(int, String)
     */
    public static ShowDialogEvent watchIsOverDialog(String showName) {
        return new ShowDialogEvent(ViewIds.SHOW_WATCH_IS_OVER_DIALOG, showName);
    }

    /**
     * Will render {@link ViewIds#SHOW_OVERRIDE_DIALOG} view.
     *
     * @see ShowDialogEvent#ShowDialogEvent(int, String)
     */
    public static ShowDialogEvent showOverrideDialog(String showName) {
        return new ShowDialogEvent(ViewIds.SHOW_OVERRIDE_DIALOG, showName);
    }

    /**
     * Construct an event, that will trigger rendering of the specified view.
     *
     * @param viewId ID of the show dialog view to render by this event
     * @param showName name of the show to be mentioned in the dialog
     */
    public ShowDialogEvent(int viewId, String showName) {
        if (StringUtils.isEmpty(showName)) {
            throw new IllegalArgumentException("Show name is empty or not specified");
        }
        this.viewId = viewId;
        this.showName = showName;
    }

    /**
     * Get name of the show, about which this dialog is about.
     *
     * @return name of the show to display
     */
    public String getShowName() {
        return showName;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getViewId() {
        return viewId;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ShowDialogEvent that = (ShowDialogEvent) o;
        return viewId == that.viewId &&
                Objects.equals(showName, that.showName);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(viewId, showName);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "ShowDialogEvent{" +
                "viewId=" + viewId +
                ", showName='" + showName + '\'' +
                '}';
    }
}
