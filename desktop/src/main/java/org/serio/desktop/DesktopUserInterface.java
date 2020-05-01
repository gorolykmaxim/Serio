package org.serio.desktop;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.datatype.jdk8.Jdk8Module;
import javafx.scene.web.WebEngine;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.UserInterface;

public class DesktopUserInterface implements UserInterface {
    private final WebEngine webEngine;
    private final ObjectMapper mapper;

    public DesktopUserInterface(WebEngine webEngine) {
        this.webEngine = webEngine;
        mapper = new ObjectMapper();
        mapper.registerModule(new Jdk8Module());
    }

    @Override
    public void sendEvent(ApplicationEvent event) {
        try {
            webEngine.executeScript("dispatchApplicationEvent(" + mapper.writeValueAsString(event) + ")");
        } catch (JsonProcessingException e) {
            throw new RuntimeException(e);
        }
    }
}
