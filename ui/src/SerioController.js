export default class SerioController {
    addShow() {
        console.info('Opening add show view');
    }
    importShowFromJson() {
        console.info('Opening import show from json view');
    }
    selectShow(showId) {
        console.info(`Opening show details view from show ${showId}`);
    }
    editThumbnailCrawler(showCrawlerId) {
        console.info(`Opening edit crawler view for thumbnail crawler of show ${showCrawlerId}`);
    }
    editEpisodeVideoCrawler(showCrawlerId) {
        console.info(`Opening edit crawler view for episode video crawler of show ${showCrawlerId}`);
    }
    editEpisodeNameCrawler(showCrawlerId) {
        console.info(`Opening edit crawler view for episode name crawler of show ${showCrawlerId}`);
    }
    back() {
        console.info('Opening previous view');
    }
    saveShowCrawler(showCrawlerId, showCrawlerName) {
        console.info(`Committing changes of show crawler ${showCrawlerId} with a name ${showCrawlerName}`);
    }
    importShowCrawler(rawShowCrawler) {
        console.info(`Importing show crawler: ${rawShowCrawler}`);
    }
    previewCrawler(showCrawlerId, crawlerType, rawCrawler) {
        console.info(`Opening crawl preview view for crawler ${showCrawlerId} with type ${crawlerType} to edit ${rawCrawler}`);
    }
    saveCrawler(showCrawlerId, crawlerType, rawCrawler) {
        console.info(`Committing changes of ${crawlerType} ${showCrawlerId}: ${rawCrawler}`);
    }
    viewCrawlLog(showCrawlerId) {
        console.info(`Opening view crawl log view of crawler ${showCrawlerId}`);
    }
    viewLogEntry(showCrawlerId, logEntryId) {
        console.info(`Opening view crawl log entry details view of crawler ${showCrawlerId} to view ${logEntryId}`);
    }
    playShow(showId) {
        console.info(`Play show ${showId}`);
    }
    playShowEpisode(showId, episodeNumber) {
        console.info(`Play episode ${episodeNumber} of show ${showId}`);
    }
    editShowCrawler(showId) {
        console.info(`Opening edit show crawler view for show ${showId}`);
    }
    shareShowCrawler(showId) {
        console.info(`Copying crawler of ${showId} to the clipboard`);
    }
    crawlShow(showId) {
        console.info(`Crawling show ${showId}`);
    }
    viewShowCrawlLog(showId) {
        console.info(`Opening view crawl log view of crawler ${showId}`);
    }
    initiateClearShowWatchHistory(showId) {
        console.info(`Opening dialog view to confirm clearing show watch history of ${showId}`);
    }
    initiateShowRemoval(showId) {
        console.info(`Opening dialog view to confirm show removal of ${showId}`);
    }
    reportShowWatchProgress(showId, progress) {
        console.info(`Reporting watch progress of ${showId} - ${progress}`);
    }
    playPreviousEpisode(showId) {
        console.info(`Going to play previous episode of ${showId}`);
    }
    playNextEpisode(showId) {
        console.info(`Going to play next episode of ${showId}`);
    }
    clearWatchHistory(showId) {
        console.info(`Clear watch history of ${showId}`);
    }
    deleteShow(showId) {
        console.info(`Delete show ${showId}`);
    }
    playShowFromTheBeginning(showId) {
        console.info(`Play show ${showId} from the beginning`);
    }
    viewAllShows() {
        console.info('Opening all shows view');
    }
}
