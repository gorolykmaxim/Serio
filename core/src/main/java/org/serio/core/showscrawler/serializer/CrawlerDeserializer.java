package org.serio.core.showscrawler.serializer;

import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.deser.std.StdDeserializer;
import org.serio.core.showscrawler.crawler.Crawler;
import org.serio.core.showscrawler.crawler.CrawlerStep;
import org.serio.core.showscrawler.crawler.CrawlerStepException;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Jackson deserializer of {@link Crawler}.
 *
 * <p>In case something is wrong with the specified crawler - an exception will be thrown, which
 * in most cases will contain information about a type of a crawler, deserialization of which has failed.</p>
 *
 * <p>In case something is wrong with on of the steps of the specified crawler - an exception will be thrown,
 * which will contain information about an index number of the invalid step.</p>
 */
public class CrawlerDeserializer extends StdDeserializer<Crawler> {
    private final CrawlerStepDeserializer crawlerStepDeserializer;

    /**
     * Construct a deserializer.
     *
     * @param crawlerStepDeserializer deserializer, that will be used to deserialize each crawler step
     */
    public CrawlerDeserializer(CrawlerStepDeserializer crawlerStepDeserializer) {
        super(Crawler.class);
        this.crawlerStepDeserializer = crawlerStepDeserializer;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Crawler deserialize(JsonParser p, DeserializationContext ctxt) throws IOException, JsonProcessingException {
        return deserialize(p.getCodec().readTree(p), (String) null);
    }

    /**
     * Deserialize the specified JSON node into a crawler.
     *
     * @param crawlerNode crawler JSON node
     * @param crawlerType type of the crawler, that is being deserialized. Used only to form a more informative
     *                    exception, thus can be specified as null.
     * @return crawler, described in the specified JSON node
     */
    public Crawler deserialize(JsonNode crawlerNode, String crawlerType) {
        if (crawlerNode == null) {
            return null;
        }
        JsonNode stepsNode = crawlerNode.get("steps");
        if (stepsNode == null || stepsNode.isNull()) {
            return new Crawler();
        }
        List<CrawlerStep> steps = new ArrayList<>();
        for (int i = 0; i < stepsNode.size(); i++) {
            try {
                steps.add(crawlerStepDeserializer.deserialize(stepsNode.get(i)));
            } catch (Exception e) {
                throw new CrawlerStepException(i + 1, crawlerType, e);
            }
        }
        return new Crawler(steps);
    }
}
