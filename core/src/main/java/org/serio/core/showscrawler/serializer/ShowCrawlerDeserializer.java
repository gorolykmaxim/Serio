package org.serio.core.showscrawler.serializer;

import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.deser.std.StdDeserializer;
import org.serio.core.showscrawler.crawler.ShowCrawler;

import java.io.IOException;
import java.util.Optional;

/**
 * Jackson deserializer of {@link ShowCrawler}.
 */
public class ShowCrawlerDeserializer extends StdDeserializer<ShowCrawler> {
    private final CrawlerDeserializer crawlerDeserializer;

    /**
     * Construct a deserializer.
     *
     * @param crawlerDeserializer deserializer, that will be used to deserialize all crawlers of a show crawler
     */
    public ShowCrawlerDeserializer(CrawlerDeserializer crawlerDeserializer) {
        super(ShowCrawler.class);
        this.crawlerDeserializer = crawlerDeserializer;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ShowCrawler deserialize(JsonParser p, DeserializationContext ctxt) throws IOException, JsonProcessingException {
        JsonNode showCrawlerNode = p.getCodec().readTree(p);
        JsonNode showNameNode = showCrawlerNode.get("showName");
        return new ShowCrawler(
                Optional.ofNullable(showNameNode).map(JsonNode::asText).orElse(null),
                crawlerDeserializer.deserialize(showCrawlerNode.get("thumbnailCrawler"), "thumbnail crawler"),
                crawlerDeserializer.deserialize(showCrawlerNode.get("episodeVideoCrawler"), "episode video crawler"),
                crawlerDeserializer.deserialize(showCrawlerNode.get("episodeNameCrawler"), "episode name crawler")
        );
    }
}
