package org.serio.core.showscrawler.serializer;

import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.deser.std.StdDeserializer;
import org.serio.core.showscrawler.crawler.CrawlerStep;
import org.serio.core.showscrawler.crawler.CrawlerStepAttributeException;
import org.serio.core.showscrawler.crawler.CrawlerStepTypeNotSpecifiedException;
import org.serio.core.showscrawler.crawler.CrawlerStepTypes;

import java.io.IOException;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

/**
 * Jackson deserializer of {@link CrawlerStep}.
 */
public class CrawlerStepDeserializer extends StdDeserializer<CrawlerStep> {
    private final CrawlerStepTypes crawlerStepTypes;

    /**
     * Construct a deserializer.
     *
     * @param crawlerStepTypes collection of all expected {@link CrawlerStep} types
     */
    public CrawlerStepDeserializer(CrawlerStepTypes crawlerStepTypes) {
        super(CrawlerStep.class);
        this.crawlerStepTypes = crawlerStepTypes;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public CrawlerStep deserialize(JsonParser p, DeserializationContext ctxt) throws IOException, JsonProcessingException {
        return deserialize(p.getCodec().readTree(p));
    }

    /**
     * @see CrawlerStepDeserializer#deserialize(JsonParser, DeserializationContext)
     */
    public CrawlerStep deserialize(JsonNode stepNode) {
        JsonNode stepTypeNode = stepNode.get("type");
        if (stepTypeNode == null || stepTypeNode.isNull()) {
            throw new CrawlerStepTypeNotSpecifiedException();
        }
        Map<String, String> properties = new HashMap<>();
        Iterable<Map.Entry<String, JsonNode>> stepFields = stepNode::fields;
        for (Map.Entry<String, JsonNode> keyValue : stepFields) {
            String fieldName = keyValue.getKey();
            JsonNode fieldValue = keyValue.getValue();
            if (!"type".equals(fieldName) && !fieldValue.isNull()) {
                properties.put(fieldName, fieldValue.asText());
            }
        }
        CrawlerStep step = new CrawlerStep(stepTypeNode.asText(), properties);
        Collection<String> missingMandatoryProperties = crawlerStepTypes.getMissingMandatoryPropertiesOf(step);
        if (!missingMandatoryProperties.isEmpty()) {
            throw new CrawlerStepAttributeException(step.getType(), missingMandatoryProperties);
        }
        return step;
    }
}
