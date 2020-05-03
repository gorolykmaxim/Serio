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

public class DesktopHttpClient implements HttpClient {
    private final WebClient webClient;

    public DesktopHttpClient() {
        TcpClient client = TcpClient.create()
                .option(ChannelOption.CONNECT_TIMEOUT_MILLIS, 5000)
                .doOnConnected(connection -> {
                    connection.addHandler(new ReadTimeoutHandler(5000, TimeUnit.MILLISECONDS));
                    connection.addHandler(new WriteTimeoutHandler(5000, TimeUnit.MILLISECONDS));
                });
        webClient = WebClient.builder()
                .clientConnector(new ReactorClientHttpConnector(reactor.netty.http.client.HttpClient.from(client)))
                .build();
    }

    @Override
    public Future<String> fetchContentFromUrl(String url) {
        return webClient.method(HttpMethod.GET).uri(url).retrieve().bodyToMono(String.class).toFuture();
    }
}
