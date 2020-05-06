package org.android.serio.storage;

public class StorageException extends RuntimeException {
    public StorageException(String intent, Throwable cause) {
        super(String.format("Failed to %s. Reason: %s", intent, cause.getMessage()), cause);
    }
}
