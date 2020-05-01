import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.datatype.jdk8.Jdk8Module;
import javafx.application.Application;
import javafx.concurrent.Worker;
import javafx.scene.Scene;
import javafx.scene.layout.StackPane;
import javafx.scene.web.WebView;
import javafx.stage.Stage;
import org.serio.core.applicationcontroller.event.ImportShowFromJsonEvent;

public class SerioDesktop extends Application {
    @Override
    public void start(Stage primaryStage) throws Exception {
        primaryStage.setTitle("Serio");
        WebView webView = new WebView();
        ObjectMapper mapper = new ObjectMapper();
        mapper.registerModule(new Jdk8Module());
        webView.getEngine().load(getClass().getResource("resources/assets/index.html").toString());
        webView.getEngine().getLoadWorker().stateProperty().addListener((observable, oldValue, newValue) -> {
            if (newValue == Worker.State.SUCCEEDED) {
                try {
                    primaryStage.show();
                    webView.getEngine().executeScript("dispatchApplicationEvent(" + mapper.writeValueAsString(new ImportShowFromJsonEvent("lelw")) + ")");
                } catch (JsonProcessingException e) {
                    e.printStackTrace();
                }
            }
        });
        primaryStage.setScene(new Scene(new StackPane(webView)));
    }

    public static void main(String[] args) {
        launch(args);
    }
}
