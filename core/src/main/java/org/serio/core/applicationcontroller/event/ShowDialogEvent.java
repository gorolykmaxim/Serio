package org.serio.core.applicationcontroller.event;

import org.apache.commons.lang3.StringUtils;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.Objects;

public class ShowDialogEvent implements ApplicationEvent {
    private final int viewId;
    private final String showName;

    public static ShowDialogEvent clearWatchHistoryDialog(String showName) {
        return new ShowDialogEvent(ViewIds.SHOW_CLEAR_WATCH_HISTORY_DIALOG, showName);
    }

    public static ShowDialogEvent deleteShowDialog(String showName) {
        return new ShowDialogEvent(ViewIds.SHOW_DELETE_SHOW_DIALOG, showName);
    }

    public static ShowDialogEvent watchIsOverDialog(String showName) {
        return new ShowDialogEvent(ViewIds.SHOW_WATCH_IS_OVER_DIALOG, showName);
    }

    public static ShowDialogEvent showOverrideDialog(String showName) {
        return new ShowDialogEvent(ViewIds.SHOW_OVERRIDE_DIALOG, showName);
    }

    public ShowDialogEvent(int viewId, String showName) {
        if (StringUtils.isEmpty(showName)) {
            throw new IllegalArgumentException("Show name is empty or not specified");
        }
        this.viewId = viewId;
        this.showName = showName;
    }

    public String getShowName() {
        return showName;
    }

    @Override
    public int getViewId() {
        return viewId;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ShowDialogEvent that = (ShowDialogEvent) o;
        return viewId == that.viewId &&
                Objects.equals(showName, that.showName);
    }

    @Override
    public int hashCode() {
        return Objects.hash(viewId, showName);
    }

    @Override
    public String toString() {
        return "ShowDialogEvent{" +
                "viewId=" + viewId +
                ", showName='" + showName + '\'' +
                '}';
    }
}
