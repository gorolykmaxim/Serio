package org.android.serio;

import com.koushikdutta.async.http.AsyncHttpClient;
import com.koushikdutta.async.http.AsyncHttpGet;
import com.koushikdutta.async.http.AsyncHttpRequest;

import org.serio.core.httpclient.HttpClient;

import java.util.concurrent.Future;

public class AndroidHttpClient implements HttpClient {
    private static final int TIMEOUT = 5000;
    private final AsyncHttpClient httpClient;

    public AndroidHttpClient() {
        this.httpClient = AsyncHttpClient.getDefaultInstance();
    }

    @Override
    public Future<String> fetchContentFromUrl(String url) {
        AsyncHttpRequest request = new AsyncHttpGet(url);
        request.setTimeout(TIMEOUT);
        return httpClient.executeString(request, null);
    }
}
