package org.serio.core.httpclient;

import java.util.concurrent.Future;

/**
 * Downloads contents of web links.
 */
public interface HttpClient {
    /**
     * Send GET request to the specified URL and return raw response body.
     *
     * @param url URL to send the request to
     * @return raw content of the URL
     */
    Future<String> fetchContentFromUrl(String url);
}
