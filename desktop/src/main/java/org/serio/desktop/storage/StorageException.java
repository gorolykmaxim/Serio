package org.serio.desktop.storage;

public class StorageException extends RuntimeException {
    public StorageException(String operation, Throwable cause) {
        super(String.format("Failed to %s. Reason: %s", operation, cause.getMessage()), cause);
    }
}
