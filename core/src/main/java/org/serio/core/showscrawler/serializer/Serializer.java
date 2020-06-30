package org.serio.core.showscrawler.serializer;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.ObjectWriter;
import com.fasterxml.jackson.databind.module.SimpleModule;
import org.apache.commons.lang3.StringUtils;
import org.serio.core.showscrawler.crawler.Crawler;
import org.serio.core.showscrawler.crawler.CrawlerStep;
import org.serio.core.showscrawler.crawler.CrawlerStepTypes;
import org.serio.core.showscrawler.crawler.ShowCrawler;

/**
 * Serializer / deserializer of {@link ShowCrawler} and {@link Crawler} instances.
 *
 * <p>Uses JSON as a serialized format.</p>
 */
public class Serializer {
    private final ObjectMapper objectMapper;
    private final ObjectWriter objectWriter;

    /**
     * Construct a serializer.
     *
     * @param crawlerStepTypes collection of all possible types of a {@link CrawlerStep}. Will be used to validate
     *                         each crawler step, being deserialized.
     */
    public Serializer(CrawlerStepTypes crawlerStepTypes) {
        objectMapper = new ObjectMapper();
        SimpleModule crawlerModule = new SimpleModule();
        CrawlerStepSerializer crawlerStepSerializer = new CrawlerStepSerializer();
        CrawlerStepDeserializer crawlerStepDeserializer = new CrawlerStepDeserializer(crawlerStepTypes);
        CrawlerDeserializer crawlerDeserializer = new CrawlerDeserializer(crawlerStepDeserializer);
        ShowCrawlerDeserializer showCrawlerDeserializer = new ShowCrawlerDeserializer(crawlerDeserializer);
        crawlerModule.addSerializer(CrawlerStep.class, crawlerStepSerializer);
        crawlerModule.addDeserializer(CrawlerStep.class, crawlerStepDeserializer);
        crawlerModule.addDeserializer(Crawler.class, crawlerDeserializer);
        crawlerModule.addDeserializer(ShowCrawler.class, showCrawlerDeserializer);
        objectMapper.registerModule(crawlerModule);
        objectWriter = objectMapper.writerWithDefaultPrettyPrinter();
    }

    /**
     * Serialize specified show crawler.
     *
     * @param showCrawler show crawler to serialize
     * @return string representation of the specified show crawler
     */
    public String serializeShowCrawler(ShowCrawler showCrawler) {
        return serialize(showCrawler);
    }

    /**
     * Re-create a show crawler from it's specified string representation.
     *
     * @param showCrawler string representation of a show crawler
     * @return deserialized show crawler
     */
    public ShowCrawler deserializeShowCrawler(String showCrawler) {
        return deserialize(showCrawler, ShowCrawler.class);
    }

    /**
     * Serialize specified crawler.
     *
     * @param crawler crawler to serialize
     * @return string representation of the specified crawler
     */
    public String serializeCrawler(Crawler crawler) {
        return serialize(crawler);
    }

    /**
     * Re-create a crawler from it's specified string representation.
     *
     * @param crawler string representation of a crawler
     * @return deserialized crawler
     */
    public Crawler deserializeCrawler(String crawler) {
        return deserialize(crawler, Crawler.class);
    }

    private String serialize(Object crawler) {
        try {
            return objectWriter.writeValueAsString(crawler);
        } catch (JsonProcessingException e) {
            throw new CrawlerSerializationException(crawler, e);
        }
    }

    private <T> T deserialize(String crawler, Class<T> type) {
        try {
            if (StringUtils.isEmpty(crawler)) {
                throw new IllegalArgumentException("Crawler is not specified");
            }
            return objectMapper.readValue(crawler, type);
        } catch (JsonProcessingException e) {
            throw new CrawlerDeserializationException(crawler, e);
        }
    }
}
