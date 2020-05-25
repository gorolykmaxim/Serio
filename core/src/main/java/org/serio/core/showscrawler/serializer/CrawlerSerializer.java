package org.serio.core.showscrawler.serializer;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.ObjectWriter;
import org.apache.commons.lang3.StringUtils;
import org.serio.core.showscrawler.crawler.Crawler;
import org.serio.core.showscrawler.crawler.ShowCrawler;
import org.serio.core.showscrawler.crawler.step.*;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * Serializer / deserializer of {@link ShowCrawler} and {@link Crawler} instances.
 *
 * <p>Uses JSON as a serialized format.</p>
 */
public class CrawlerSerializer {
    private final ObjectWriter writer;
    private final ObjectMapper mapper;

    /**
     * Construct a serializer.
     */
    public CrawlerSerializer() {
        mapper = new ObjectMapper();
        writer = mapper.writerWithDefaultPrettyPrinter();
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
     * Serialize specified crawler.
     *
     * @param crawler crawler to serialize
     * @return string representation of the specified crawler
     */
    public String serializeCrawler(Crawler crawler) {
        return serialize(crawler);
    }

    /**
     * Re-create a show crawler from it's specified string representation.
     *
     * @param serializedShowCrawler string representation of a show crawler
     * @return deserialized show crawler
     */
    public ShowCrawler deserializeShowCrawler(String serializedShowCrawler) {
        try {
            if (StringUtils.isEmpty(serializedShowCrawler)) {
                throw new IllegalArgumentException("Show crawler is not specified");
            }
            Map<String, Object> rawShowCrawler = deserialize(serializedShowCrawler, Map.class);
            return new ShowCrawler(
                    (String) rawShowCrawler.get("showName"),
                    createCrawlerFromMap((Map<String, Object>) rawShowCrawler.get("thumbnailCrawler"), "thumbnail"),
                    createCrawlerFromMap((Map<String, Object>) rawShowCrawler.get("episodeVideoCrawler"), "episode video"),
                    createCrawlerFromMap((Map<String, Object>) rawShowCrawler.get("episodeNameCrawler"), "episode name")
            );
        } catch (Exception e) {
            throw new CrawlerDeserializationException(serializedShowCrawler, e);
        }
    }

    /**
     * Re-create a crawler from it's specified string representation.
     *
     * @param serializedCrawler string representation of a crawler
     * @return deserialized crawler
     */
    public Crawler deserializeCrawler(String serializedCrawler) {
        try {
            if (StringUtils.isEmpty(serializedCrawler)) {
                throw new IllegalArgumentException("Crawler is not specified");
            }
            return createCrawlerFromMap(deserialize(serializedCrawler, Map.class), null);
        } catch (Exception e) {
            throw new CrawlerDeserializationException(serializedCrawler, e);
        }
    }

    private Crawler createCrawlerFromMap(Map<String, Object> rawCrawler, String crawlerType) {
        if (rawCrawler == null) {
            return null;
        }
        List<Map<String, String>> rawSteps = (List<Map<String, String>>) rawCrawler.get("steps");
        if (rawSteps == null) {
            return new Crawler();
        }
        List<CrawlerStep> steps = new ArrayList<>(rawSteps.size());
        for (int i = 0; i < rawSteps.size(); i++) {
            try {
                Map<String, String> rawStep = rawSteps.get(i);
                String type = rawStep.get("type");
                if (type == null) {
                    throw new CrawlerStepTypeNotSpecifiedException();
                }
                if (type.equals(ValueStep.TYPE)) {
                    steps.add(new ValueStep(rawStep.get("value")));
                } else if (type.equals(FetchStep.TYPE)) {
                    steps.add(new FetchStep());
                } else if (type.equals(TransformStep.TYPE)) {
                    steps.add(new TransformStep(rawStep.get("template")));
                } else if (type.equals(RegExpStep.TYPE)) {
                    steps.add(new RegExpStep(rawStep.get("regExp")));
                } else {
                    throw new UnknownCrawlerStepTypeException(type, ValueStep.TYPE, FetchStep.TYPE, TransformStep.TYPE, RegExpStep.TYPE);
                }
            } catch (Exception e) {
                throw new CrawlerStepException(i + 1, crawlerType, e);
            }
        }
        return new Crawler(steps);
    }

    private String serialize(Object crawler) {
        try {
            return writer.writeValueAsString(crawler);
        } catch (Exception e) {
            throw new CrawlerSerializationException(crawler, e);
        }
    }

    private  <T> T deserialize(String crawler, Class<T> type) throws JsonProcessingException {
        return mapper.readValue(crawler, type);
    }
}
