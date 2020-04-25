package org.serio.core.clipboard;

/**
 * Loads data to the system's clipboard buffer.
 */
public interface Clipboard {
    /**
     * Copy the specified string content to the system's clipboard.
     *
     * @param content content to copy
     */
    void setContent(String content);
}
