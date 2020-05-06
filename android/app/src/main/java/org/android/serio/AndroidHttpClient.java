package org.android.serio;

import com.koushikdutta.async.http.AsyncHttpClient;
import com.koushikdutta.async.http.AsyncHttpGet;
import com.koushikdutta.async.http.AsyncHttpRequest;

import org.serio.core.httpclient.HttpClient;

import java.util.concurrent.Future;

/**
 * Android implementation of {@link HttpClient}.
 *
 * <p>Uses a non-blocking single-threaded event loop based client {@link AsyncHttpClient}.</p>
 */
public class AndroidHttpClient implements HttpClient {
    private static final int TIMEOUT = 5000;
    private final AsyncHttpClient httpClient;

    /**
     * Construct an http client.
     */
    public AndroidHttpClient() {
        this.httpClient = AsyncHttpClient.getDefaultInstance();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Future<String> fetchContentFromUrl(String url) {
        AsyncHttpRequest request = new AsyncHttpGet(url);
        request.setTimeout(TIMEOUT);
        return httpClient.executeString(request, null);
    }
}
