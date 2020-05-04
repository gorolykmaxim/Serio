package org.serio.desktop;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.datatype.jdk8.Jdk8Module;
import javafx.application.Platform;
import javafx.scene.web.WebEngine;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.UserInterface;

/**
 * Desktop implementation of {@link UserInterface}.
 *
 * <p>Hosts the user interface in a {@link javafx.scene.web.WebView} and sends events to it via
 * {@link WebEngine#executeScript(String)}.</p>
 *
 * <p>{@link UserInterface#sendEvent(ApplicationEvent)} of this implementation is expected to be executed
 * from a non-JavaFX thread, thus the interaction with the actual webview occurs on the JavaFX thread via
 * {@link Platform#runLater(Runnable)}. Though this implementation can be called from the main thread as well.</p>
 */
public class DesktopUserInterface implements UserInterface {
    private final WebEngine webEngine;
    private final ObjectMapper mapper;

    /**
     * Construct a user interface.
     *
     * @param webEngine web engine, via which application events will be send to the user interface
     */
    public DesktopUserInterface(WebEngine webEngine) {
        this.webEngine = webEngine;
        mapper = new ObjectMapper();
        mapper.registerModule(new Jdk8Module());
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void sendEvent(ApplicationEvent event) {
        Platform.runLater(() -> {
            try {
                webEngine.executeScript("dispatchApplicationEvent(" + mapper.writeValueAsString(event) + ")");
            } catch (JsonProcessingException e) {
                throw new RuntimeException(e);
            }
        });
    }
}
