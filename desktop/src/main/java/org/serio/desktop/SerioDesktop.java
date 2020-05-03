package org.serio.desktop;

import javafx.application.Application;
import javafx.concurrent.Worker;
import javafx.scene.Scene;
import javafx.scene.layout.StackPane;
import javafx.scene.web.WebView;
import javafx.stage.Stage;
import netscape.javascript.JSObject;
import org.apache.commons.io.IOUtils;
import org.serio.core.Core;
import org.serio.core.applicationcontroller.ApplicationController;
import org.serio.core.httpclient.HttpClient;
import org.serio.core.userinterface.UserInterface;
import org.serio.desktop.platform.DesktopPlatform;
import org.serio.desktop.platform.Platforms;
import org.serio.desktop.storage.DesktopStorage;
import org.sqlite.SQLiteDataSource;

import java.net.URL;
import java.nio.charset.Charset;
import java.nio.file.Paths;
import java.util.Properties;

public class SerioDesktop extends Application {
    @Override
    public void start(Stage primaryStage) throws Exception {
        primaryStage.setTitle("Serio");
        WebView webView = new WebView();
        webView.setContextMenuEnabled(false);
        UserInterface userInterface = new DesktopUserInterface(webView.getEngine());
        HttpClient httpClient = new DesktopHttpClient();
        DesktopPlatform platform = Platforms.getCurrentPlatform("Serio");
        Properties storageQueries = new Properties();
        storageQueries.load(getClass().getClassLoader().getResourceAsStream("storage/queries.properties"));
        String storageInitializationQuery = IOUtils.resourceToString("storage/schema.sql", Charset.defaultCharset(), getClass().getClassLoader());
        SQLiteDataSource dataSource = new SQLiteDataSource();
        dataSource.setEnforceForeinKeys(true);
        dataSource.setUrl("jdbc:sqlite:" + Paths.get(System.getProperty("user.home"), ".serio.db").toString());
        DesktopStorage storage = new DesktopStorage(dataSource, storageQueries, storageInitializationQuery);
        storage.initialize();
        Core core = new Core(platform, httpClient, platform, storage, storage, storage, userInterface, storage);
        URL uiEntryPoint = getClass().getClassLoader().getResource("assets/index.html");
        webView.getEngine().load(uiEntryPoint.toString() + "#platform=0&runtimeType=0");
        webView.getEngine().getLoadWorker().stateProperty().addListener((observable, oldValue, newValue) -> {
            if (newValue == Worker.State.SUCCEEDED) {
                primaryStage.show();
                ApplicationController controller = core.initializeApplicationController();
                JSObject window = (JSObject) webView.getEngine().executeScript("window");
                window.setMember("serioController", controller);
            }
        });
        primaryStage.setScene(new Scene(new StackPane(webView), 800, 600));
    }

    public static void main(String[] args) {
        launch(args);
    }
}
