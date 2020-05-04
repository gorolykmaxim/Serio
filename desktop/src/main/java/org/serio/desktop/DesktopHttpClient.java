package org.serio.desktop;

import io.netty.channel.ChannelOption;
import io.netty.handler.timeout.ReadTimeoutHandler;
import io.netty.handler.timeout.WriteTimeoutHandler;
import org.serio.core.httpclient.HttpClient;
import org.springframework.http.HttpMethod;
import org.springframework.http.client.reactive.ReactorClientHttpConnector;
import org.springframework.web.reactive.function.client.WebClient;
import reactor.netty.tcp.TcpClient;

import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

/**
 * Desktop implementation of {@link HttpClient}.
 *
 * <p>Uses spring-webflux's {@link WebClient} as a transport. Has a default connection / read / write timeout
 * set to {@link DesktopHttpClient#TIMEOUT} in milliseconds. Does not constraint the underlying connection
 * pool's size.</p>
 */
public class DesktopHttpClient implements HttpClient {
    public static final int TIMEOUT = 5000;

    private final WebClient webClient;

    /**
     * Construct an http client.
     */
    public DesktopHttpClient() {
        TcpClient client = TcpClient.create()
                .option(ChannelOption.CONNECT_TIMEOUT_MILLIS, TIMEOUT)
                .doOnConnected(connection -> {
                    connection.addHandler(new ReadTimeoutHandler(TIMEOUT, TimeUnit.MILLISECONDS));
                    connection.addHandler(new WriteTimeoutHandler(TIMEOUT, TimeUnit.MILLISECONDS));
                });
        webClient = WebClient.builder()
                .clientConnector(new ReactorClientHttpConnector(reactor.netty.http.client.HttpClient.from(client)))
                .build();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Future<String> fetchContentFromUrl(String url) {
        return webClient.method(HttpMethod.GET).uri(url).retrieve().bodyToMono(String.class).toFuture();
    }
}
