package org.serio.core.watchhistory;

import java.util.Objects;

/**
 * Progress of a tv show episode being watched by the user.
 */
public class WatchProgress {
    private final double percentage;

    /**
     * Indicates that the corresponding episode has not been watched yet.
     */
    public static final WatchProgress NONE = new WatchProgress(0);
    /**
     * Indicates that the corresponding episode has been watched from start to finish.
     */
    public static final WatchProgress COMPLETE = new WatchProgress(100);

    /**
     * Create an episode watch progress from the specified percentage.
     *
     * @param percentage percentage of episode's duration, that have already been watched by the user.
     *                   Must be equal or greater than 0 and less than 100.
     * @return corresponding episode watch progress
     */
    public static WatchProgress fromPercentage(double percentage) {
        if (percentage == 0) {
            return NONE;
        } else if (percentage == 100) {
            return COMPLETE;
        } else {
            return new WatchProgress(percentage);
        }
    }

    private WatchProgress(double percentage) {
        if (percentage < 0 || percentage > 100) {
            throw new IllegalArgumentException("Watch progress percentage should be greater than 0 and less than 100. Specified - " + percentage);
        }
        this.percentage = percentage;
    }

    /**
     * Get episode watch progress as a percentage of the total amount of episodes' duration being watched.
     *
     * @return watch progress as a percentage
     */
    public double getPercentage() {
        return percentage;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        WatchProgress that = (WatchProgress) o;
        return Double.compare(that.percentage, percentage) == 0;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(percentage);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "WatchProgress{" +
                "percentage=" + percentage +
                '}';
    }
}
