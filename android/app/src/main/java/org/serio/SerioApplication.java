package org.serio;

import android.app.Application;

import java.io.File;

public class SerioApplication extends Application {
    public Core core;

    private String prepareDatabasePath() {
        File databaseDir = new File(getFilesDir().getParentFile(), "databases");
        databaseDir.mkdirs();
        File databasePath = new File(databaseDir, "serio.db");
        return databasePath.getAbsolutePath();
    }

    @Override
    public void onCreate() {
        super.onCreate();
        core = new Core(prepareDatabasePath());
        core.start();
    }
}
