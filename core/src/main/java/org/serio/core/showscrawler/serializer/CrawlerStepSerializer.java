package org.serio.core.showscrawler.serializer;

import com.fasterxml.jackson.core.JsonGenerator;
import com.fasterxml.jackson.databind.SerializerProvider;
import com.fasterxml.jackson.databind.ser.std.StdSerializer;
import org.serio.core.showscrawler.crawler.CrawlerStep;

import java.io.IOException;
import java.util.Map;

/**
 * Jackson serializer of {@link CrawlerStep}.
 */
public class CrawlerStepSerializer extends StdSerializer<CrawlerStep> {
    /**
     * Construct a serializer.
     */
    public CrawlerStepSerializer() {
        super(CrawlerStep.class);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void serialize(CrawlerStep value, JsonGenerator gen, SerializerProvider provider) throws IOException {
        gen.writeStartObject();
        for (Map.Entry<String, String> properties : value.getProperties().entrySet()) {
            gen.writeStringField(properties.getKey(), properties.getValue());
        }
        gen.writeStringField("type", value.getType());
        gen.writeEndObject();
    }
}
