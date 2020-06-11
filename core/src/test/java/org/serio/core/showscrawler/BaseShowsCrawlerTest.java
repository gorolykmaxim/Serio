package org.serio.core.showscrawler;

import org.junit.Before;
import org.serio.core.httpclient.HttpClient;
import org.serio.core.showcrawlerlogstorage.ShowCrawlerLogStorage;
import org.serio.core.showcrawlerstorage.ShowCrawlerStorage;
import org.serio.core.showstorage.Show;
import org.serio.core.taskexecutor.TaskExecutor;

import java.util.Collections;
import java.util.concurrent.Callable;
import java.util.concurrent.CompletableFuture;

import static org.mockito.Mockito.*;

public abstract class BaseShowsCrawlerTest {
    protected static final String SHOW_NAME = "Game of Thrones";
    protected static final String SHOW_URL = "http://tv-shows.com/game-of-thrones";
    protected static final String COMPLETE_SHOW_CRAWLER_TEMPLATE = "{\n" +
            "  \"showName\" : \"%s\",\n" +
            "  \"thumbnailCrawler\" : %s,\n" +
            "  \"episodeVideoCrawler\" : %s,\n" +
            "  \"episodeNameCrawler\" : %s\n" +
            "}";
    protected static final String BARE_MINIMUM_SHOW_CRAWLER_TEMPLATE = "{\n" +
            "  \"showName\" : \"%s\",\n" +
            "  \"episodeVideoCrawler\" : %s\n" +
            "}";
    protected static final String SHOW_CRAWLER_WITHOUT_SHOW_NAME = "{\n" +
            "  \"thumbnailCrawler\" : %s,\n" +
            "  \"episodeVideoCrawler\" : %s,\n" +
            "  \"episodeNameCrawler\" : %s\n" +
            "}";
    protected static final String SHOW_CRAWLER_WITHOUT_EPISODE_VIDEO_CRAWLER = "{\n" +
            "  \"showName\" : \"Game of Thrones\",\n" +
            "  \"thumbnailCrawler\" : %s,\n" +
            "  \"episodeNameCrawler\" : %s\n" +
            "}";

    protected static final String CRAWLER_WITHOUT_STEPS = "{}";
    protected static final String CRAWLER_WITH_EMPTY_STEPS = "{\n" +
            "  \"steps\" : [ ]\n" +
            "}";
    protected static final String COMPLETE_CRAWLER_TEMPLATE = "{\n" +
            "  \"steps\" : [ {\n" +
            "    \"value\" : \"%s\",\n" +
            "    \"type\" : \"value\"\n" +
            "  }, {\n" +
            "    \"type\" : \"fetch\"\n" +
            "  }, {\n" +
            "    \"regExp\" : \"%s\",\n" +
            "    \"type\" : \"regExp\"\n" +
            "  }, {\n" +
            "    \"template\" : \"%s\",\n" +
            "    \"type\" : \"transform\"\n" +
            "  } ]\n" +
            "}";
    protected static final String COMPLETE_CRAWLER = String.format(
            COMPLETE_CRAWLER_TEMPLATE,
            SHOW_URL,
            "(.*).mp4",
            "https://%s"
    );
    protected static final String COMPLETE_THUMBNAIL_CRAWLER = String.format(
            COMPLETE_CRAWLER_TEMPLATE,
            SHOW_URL,
            "<img src=\\\"(.*)\\\">",
            "http://tv-shows.com/game-of-thrones/%s"
    );
    protected static final String COMPLETE_VIDEO_CRAWLER = String.format(
            COMPLETE_CRAWLER_TEMPLATE,
            SHOW_URL,
            "<video src=\\\"(.*)\\\"></video> next - <video src=\\\"(.*)\\\"></video>",
            "http://tv-shows.com/game-of-thrones/%s"
    );
    protected static final String COMPLETE_NAME_CRAWLER_TEMPLATE = "{\n" +
            "  \"steps\" : [ {\n" +
            "    \"regExp\" : \"%s\",\n" +
            "    \"type\" : \"regExp\"\n" +
            "  }, {\n" +
            "    \"template\" : \"%s\",\n" +
            "    \"type\" : \"transform\"\n" +
            "  } ]\n" +
            "}";
    protected static final String COMPLETE_NAME_CRAWLER = String.format(
            COMPLETE_NAME_CRAWLER_TEMPLATE,
            "http://tv-shows.com/game-of-thrones/(.*).mp4",
            "The %s"
    );
    protected static final String CRAWLER_WITH_STEP_WITHOUT_TYPE = "{\n" +
            "  \"steps\" : [ {\n" +
            "    \"value\" : \"http://tv-shows.com/game-of-thrones\",\n" +
            "    \"type\" : \"value\"\n" +
            "  }, {\n" +
            "  }, {\n" +
            "    \"regExp\" : \"(.*).mp4\",\n" +
            "    \"type\" : \"regExp\"\n" +
            "  }, {\n" +
            "    \"template\" : \"https://%s\",\n" +
            "    \"type\" : \"transform\"\n" +
            "  } ]\n" +
            "}";
    protected static final String CRAWLER_WITH_STEP_WITH_UNKNOWN_TYPE = "{\n" +
            "  \"steps\" : [ {\n" +
            "    \"value\" : \"http://tv-shows.com/game-of-thrones\",\n" +
            "    \"type\" : \"value\"\n" +
            "  }, {\n" +
            "    \"type\" : \"unknown\"\n" +
            "  }, {\n" +
            "    \"regExp\" : \"(.*).mp4\",\n" +
            "    \"type\" : \"regExp\"\n" +
            "  }, {\n" +
            "    \"template\" : \"https://%s\",\n" +
            "    \"type\" : \"transform\"\n" +
            "  } ]\n" +
            "}";
    protected static final String CRAWLER_WITH_VALUE_STEP_WITHOUT_VALUE = "{\n" +
            "  \"steps\" : [ {\n" +
            "    \"type\" : \"value\"\n" +
            "  }, {\n" +
            "    \"type\" : \"fetch\"\n" +
            "  }, {\n" +
            "    \"regExp\" : \"(.*).mp4\",\n" +
            "    \"type\" : \"regExp\"\n" +
            "  }, {\n" +
            "    \"template\" : \"https://%s\",\n" +
            "    \"type\" : \"transform\"\n" +
            "  } ]\n" +
            "}";
    protected static final String CRAWLER_WITH_REGEXP_STEP_WITHOUT_REGEXP = "{\n" +
            "  \"steps\" : [ {\n" +
            "    \"value\" : \"http://tv-shows.com/game-of-thrones\",\n" +
            "    \"type\" : \"value\"\n" +
            "  }, {\n" +
            "    \"type\" : \"fetch\"\n" +
            "  }, {\n" +
            "    \"type\" : \"regExp\"\n" +
            "  }, {\n" +
            "    \"template\" : \"https://%s\",\n" +
            "    \"type\" : \"transform\"\n" +
            "  } ]\n" +
            "}";
    protected static final String CRAWLER_WITH_TRANSFORM_STEP_WITHOUT_TEMPLATE = "{\n" +
            "  \"steps\" : [ {\n" +
            "    \"value\" : \"http://tv-shows.com/game-of-thrones\",\n" +
            "    \"type\" : \"value\"\n" +
            "  }, {\n" +
            "    \"type\" : \"fetch\"\n" +
            "  }, {\n" +
            "    \"regExp\" : \"(.*).mp4\",\n" +
            "    \"type\" : \"regExp\"\n" +
            "  }, {\n" +
            "    \"type\" : \"transform\"\n" +
            "  } ]\n" +
            "}";

    protected static final String SHOW_WEB_PAGE = "<!DOCTYPE html>\n" +
            "<html>\n" +
            "<head>\n" +
            "    <title>Dummy show page</title>\n" +
            "</head>\n" +
            "<body>\n" +
            "<div>\n" +
            "    <h1>Game of Thrones</h1>\n" +
            "    <img src=\"thumbnail.png\">\n" +
            "    <h4>Episodes:</h4>\n" +
            "    <ul>\n" +
            "        <li>\n" +
            "            <video src=\"episode-1.mp4\"></video> next - <video src=\"episode-2.mp4\"></video>\n" +
            "            <video src=\"episode-3.mp4\"></video> next - <video src=\"episode-4.mp4\"></video>\n" +
            "            <video src=\"episode-5.mp4\"></video> next - <video src=\"episode-6.mp4\"></video>\n" +
            "            <video src=\"episode-7.mp4\"></video> next - <video src=\"episode-8.mp4\"></video>\n" +
            "            <video src=\"episode-9.mp4\"></video> next - <video src=\"episode-10.mp4\"></video>\n" +
            "        </li>\n" +
            "    </ul>\n" +
            "</div>\n" +
            "</body>\n" +
            "</html>";

    protected ShowCrawlerStorage showCrawlerStorage;
    protected ShowCrawlerLogStorage showCrawlerLogStorage;
    protected HttpClient httpClient;
    protected long crawlerLogDetailsLength;
    protected String showId;
    protected Show expectedShow;
    protected ShowsCrawler showsCrawler;

    @Before
    public void setUp() throws Exception {
        expectedShow = Show.createNew(SHOW_NAME, Collections.emptyList());
        showId = expectedShow.getId().toString();
        crawlerLogDetailsLength = 15;
        showCrawlerStorage = mock(ShowCrawlerStorage.class);
        showCrawlerLogStorage = mock(ShowCrawlerLogStorage.class);
        httpClient = mock(HttpClient.class);
        TaskExecutor executor = mock(TaskExecutor.class);
        when(executor.execute(any()))
                .thenAnswer(invocation -> CompletableFuture.completedFuture(((Callable<CrawlingResult>) invocation.getArgument(0)).call()));
        when(httpClient.fetchContentFromUrl(SHOW_URL)).thenReturn(CompletableFuture.completedFuture(SHOW_WEB_PAGE));
        showsCrawler = new ShowsCrawler(showCrawlerStorage, showCrawlerLogStorage, httpClient, executor, crawlerLogDetailsLength);
    }
}
