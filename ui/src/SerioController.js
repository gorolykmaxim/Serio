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
    editThumbnailCrawler() {
        console.info('Opening edit crawler view for thumbnail crawler of the currently selected show');
    }
    editEpisodeVideoCrawler() {
        console.info('Opening edit crawler view for episode video crawler of the currently selected show');
    }
    editEpisodeNameCrawler() {
        console.info('Opening edit crawler view for episode name crawler of the currently selected show');
    }
    back() {
        console.info('Opening previous view');
    }
    saveShowCrawler(showCrawlerName) {
        console.info(`Committing changes of the currently selected show crawler with a name ${showCrawlerName}`);
    }
    importShowCrawler(rawShowCrawler) {
        console.info(`Importing show crawler: ${rawShowCrawler}`);
    }
    previewCrawler(rawCrawler) {
        console.info(`Opening crawl preview view for the currently selected crawler to edit ${rawCrawler}`);
    }
    saveCrawler(rawCrawler) {
        console.info(`Committing changes of the currently selected crawler: ${rawCrawler}`);
    }
    viewCrawlLog() {
        console.info('Opening view crawl log view of the currently selected crawler');
    }
    viewLogEntry(logEntryId) {
        console.info(`Opening view crawl log entry details view of the currently selected crawler to view ${logEntryId}`);
    }
    playShow() {
        console.info('Play the currently selected show');
    }
    playShowEpisode(episodeNumber) {
        console.info(`Play episode ${episodeNumber} of the currently selected show`);
    }
    editShowCrawler() {
        console.info('Opening edit show crawler view for the currently selected show');
    }
    shareShowCrawler() {
        console.info('Copying crawler of the currently selected show to the clipboard');
    }
    crawlShow() {
        console.info('Crawling the currently selected show');
    }
    viewShowCrawlLog() {
        console.info('Opening view crawl log view of crawler of the currently selected show');
    }
    initiateClearShowWatchHistory() {
        console.info('Opening dialog view to confirm clearing show watch history of the currently selected show');
    }
    initiateShowRemoval() {
        console.info('Opening dialog view to confirm show removal of the currently selected show');
    }
    reportShowWatchProgress(progress) {
        console.info(`Reporting watch progress of of the currently selected show - ${progress}%`);
    }
    playPreviousEpisode() {
        console.info('Going to play previous episode of the currently selected show');
    }
    playNextEpisode() {
        console.info('Going to play next episode of the currently selected show');
    }
    clearWatchHistory() {
        console.info('Clear watch history of the currently selected show');
    }
    deleteShow() {
        console.info('Delete the currently selected show');
    }
    playShowFromTheBeginning() {
        console.info('Play the currently selected show from the beginning');
    }
    viewAllShows() {
        console.info('Opening all shows view');
    }
    confirmShowOverride() {
        console.info('Confirming the override of an existing show with the currently configured one');
    }
}
