function crawl() {
    let results = http([{url: 'https://ru.wikipedia.org/wiki//dev/null'}]);
    results = http([{url: 'https://ru.wikipedia.org/wiki//dev/null'}]);
    results = http([{url: 'https://ru.wikipedia.org/wiki//dev/null'}]);
    results = http([{url: 'https://ru.wikipedia.org/wiki//dev/null'}]);
    results = http([{url: 'https://ru.wikipedia.org/wiki//dev/null'}]);
    return regex('class="(.*)"', results);
}