function crawl() {
    let results = httpRequests([{url: 'https://ru.wikipedia.org/wiki//dev/null'}]);
    results = httpRequests([{url: 'https://ru.wikipedia.org/wiki//dev/null'}]);
    results = httpRequests([{url: 'https://ru.wikipedia.org/wiki//dev/null'}]);
    results = httpRequests([{url: 'https://ru.wikipedia.org/wiki//dev/null'}]);
    results = httpRequests([{url: 'https://ru.wikipedia.org/wiki//dev/null'}]);
    return regExp('class="(.*)"', results);
}