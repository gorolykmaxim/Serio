package org.serio.desktop;

import javafx.application.Application;
import javafx.concurrent.Worker;
import javafx.scene.Scene;
import javafx.scene.layout.StackPane;
import javafx.scene.web.WebEngine;
import javafx.scene.web.WebView;
import javafx.stage.Stage;
import netscape.javascript.JSObject;
import org.apache.commons.io.IOUtils;
import org.serio.core.Core;
import org.serio.core.applicationcontroller.ApplicationController;
import org.serio.core.userinterface.UserInterface;
import org.serio.desktop.platform.DesktopPlatform;
import org.serio.desktop.platform.Platforms;
import org.serio.desktop.storage.DesktopStorage;
import org.sqlite.SQLiteDataSource;

import java.net.URL;
import java.nio.charset.Charset;
import java.util.Properties;

/**
 * The desktop version of the Serio application.
 *
 * <p>This is the application's entry point and the place where all the modules are being wired-up together.</p>
 */
public class SerioDesktop extends Application {
    private Core core;

    /**
     * {@inheritDoc}
     */
    @Override
    public void start(Stage primaryStage) throws Exception {
        ClassLoader classLoader = getClass().getClassLoader();
        // Initialize http client
        DesktopHttpClient httpClient = new DesktopHttpClient();
        // Initialize platform
        DesktopPlatform platform = Platforms.getCurrentPlatform("Serio");
        // Initialize storage
        Properties storageQueries = new Properties();
        storageQueries.load(classLoader.getResourceAsStream("storage/queries.properties"));
        String storageInitializationQuery = IOUtils.resourceToString("storage/schema.sql", Charset.defaultCharset(), getClass().getClassLoader());
        SQLiteDataSource dataSource = new SQLiteDataSource();
        dataSource.setEnforceForeinKeys(true);
        dataSource.setUrl("jdbc:sqlite:" + platform.getDatabasePath().toString());
        DesktopStorage storage = new DesktopStorage(dataSource, storageQueries, storageInitializationQuery);
        storage.initialize();
        // Initialize user interface
        WebView webView = new WebView();
        webView.setContextMenuEnabled(false);
        WebEngine webEngine = webView.getEngine();
        UserInterface userInterface = new DesktopUserInterface(webEngine);
        // Initialize core and the application controller
        core = new Core(platform, httpClient, platform, storage, storage, storage, userInterface, storage);
        ApplicationController controller = core.getApplicationController();
        // Bootstrap the application
        URL uiEntryPoint = classLoader.getResource("assets/index.html");
        webEngine.load(String.format("%s#platform=0&runtimeType=0", uiEntryPoint));
        webEngine.getLoadWorker().stateProperty().addListener((observable, oldValue, newValue) -> {
            if (newValue == Worker.State.SUCCEEDED) {
                primaryStage.show();
                controller.viewAllShows();
                JSObject window = (JSObject) webEngine.executeScript("window");
                window.setMember("serioController", controller);
            }
        });
        primaryStage.setTitle("Serio");
        primaryStage.setScene(new Scene(new StackPane(webView), 800, 600));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void stop() throws Exception {
        core.close();
    }

    public static void main(String[] args) {
        launch(args);
    }
}
