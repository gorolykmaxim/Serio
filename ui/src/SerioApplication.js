import React from "react";
import AllShows from "./views/AllShows";
import EditShowCrawler from "./views/EditShowCrawler";
import ImportShowCrawler from "./views/ImportShowCrawler";
import EditCrawler from "./views/EditCrawler";
import CrawlPreview from "./views/CrawlPreview";
import CrawlLog from "./views/CrawlLog";
import CrawlLogEntryDetails from "./views/CrawlLogEntryDetails";
import CrawlingInProgress from "./views/CrawlingInProgress";
import ShowDetails from "./views/ShowDetails";
import ShowPlayer from "./views/ShowPlayer";
import Logo from "./views/Logo";
import Dialog from "./views/Dialog";

const EVENT_TYPE = 'serio-event';

window.dispatchApplicationEvent = function(data) {
    window.dispatchEvent(new CustomEvent(EVENT_TYPE, {detail: data}));
}

export const ALL_SHOWS = 0;
export const EDIT_SHOW_CRAWLER = 1;
export const IMPORT_SHOW_CRAWLER = 2;
export const EDIT_CRAWLER = 3;
export const CRAWL_PREVIEW = 4;
export const CRAWL_LOG = 5;
export const CRAWL_LOG_ENTRY_DETAILS = 6;
export const CRAWLING_IN_PROGRESS = 7;
export const SHOW_DETAILS = 8;
export const SHOW_PLAYER = 9;
export const SHOW_CLEAR_WATCH_HISTORY_DIALOG = 10;
export const SHOW_DELETE_SHOW_DIALOG = 11;
export const SHOW_WATCH_IS_OVER_DIALOG = 12;
export const SHOW_ERROR_DIALOG = 13;

export class SerioApplication extends React.Component {
    constructor(props, context) {
        super(props, context);
        this.state = {data: {}};
        this.handleEvent = this.handleEvent.bind(this);
    }
    get isTv() {
        return this.props.isTv;
    }
    componentDidMount() {
        window.addEventListener(EVENT_TYPE, this.handleEvent);
    }
    componentWillUnmount() {
        window.removeEventListener(EVENT_TYPE, this.handleEvent);
    }
    handleEvent(event) {
        this.setState({data: event.detail});
    }
    render() {
        const viewId = this.state.data.viewId;
        switch (viewId) {
            case ALL_SHOWS:
                return this.renderAllShows();
            case EDIT_SHOW_CRAWLER:
                return this.renderEditShowCrawler();
            case IMPORT_SHOW_CRAWLER:
                return this.renderImportShowCrawler();
            case EDIT_CRAWLER:
                return this.renderEditCrawler();
            case CRAWL_PREVIEW:
                return this.renderCrawlPreview();
            case CRAWL_LOG:
                return this.renderCrawlLog();
            case CRAWL_LOG_ENTRY_DETAILS:
                return this.renderCrawlLogEntryDetails();
            case CRAWLING_IN_PROGRESS:
                return this.renderCrawlingInProgress();
            case SHOW_DETAILS:
                return this.renderShowDetails();
            case SHOW_PLAYER:
                return this.renderShowPlayer();
            case SHOW_CLEAR_WATCH_HISTORY_DIALOG:
                return this.renderClearWatchHistoryDialog();
            case SHOW_DELETE_SHOW_DIALOG:
                return this.renderDeleteShowDialog();
            case SHOW_WATCH_IS_OVER_DIALOG:
                return this.renderWatchIsOverDialog();
            case SHOW_ERROR_DIALOG:
                return this.renderErrorDialog();
            default:
                if (!viewId) {
                    return <Logo/>;
                } else {
                    console.error(`Unknown view ID: ${viewId}. Skipping...`);
                }
        }
    }
    renderAllShows() {
        const {allShows, lastWatchedShows} = this.state.data;
        return <AllShows allShows={allShows}
                         lastWatchedShows={lastWatchedShows}
                         hoverableShowCards={!this.isTv}
                         scrollOnFocus={this.isTv}
                         onAddShow={window.serioController.addShow}
                         onImportShowFromJson={window.serioController.importShowFromJson}
                         onShowSelect={(show) => window.serioController.selectShow(show.id)}/>;
    }
    renderEditShowCrawler() {
        const {showId, showName} = this.state.data;
        return <EditShowCrawler showId={showId}
                                showName={showName}
                                onEditThumbnailCrawler={() => window.serioController.editThumbnailCrawler(showId)}
                                onEditEpisodeVideoCrawler={() => window.serioController.editEpisodeVideoCrawler(showId)}
                                onEditEpisodeNameCrawler={() => window.serioController.editEpisodeNameCrawler(showId)}
                                onCancel={window.serioController.back}
                                onSave={(showName) => window.serioController.saveShowCrawler(showId, showName)}/>;
    }
    renderImportShowCrawler() {
        return <ImportShowCrawler onImport={window.serioController.importShowCrawler}
                                  onCancel={window.serioController.back}/>;
    }
    renderEditCrawler() {
        const {crawlerType, crawler} = this.state.data;
        return <EditCrawler crawlerType={crawlerType}
                            crawler={crawler}
                            onCancel={window.serioController.back}
                            onPreview={rawCrawler => window.serioController.previewCrawler(rawCrawler)}
                            onSave={rawCrawler => window.serioController.saveCrawler(rawCrawler)}/>;
    }
    renderCrawlPreview() {
        const {crawlerType, crawlItems, showCrawlerId} = this.state.data;
        return <CrawlPreview crawlerType={crawlerType}
                             crawlItems={crawlItems}
                             hoverableCrawlItems={!this.isTv}
                             scrollOnFocus={this.isTv}
                             onBack={window.serioController.back}
                             onViewLog={() => window.serioController.viewCrawlLog(showCrawlerId)}/>;
    }
    renderCrawlLog() {
        const {crawlType: crawlerType, logEntries, showCrawlerId} = this.state.data;
        return <CrawlLog crawlerType={crawlerType}
                         logEntries={logEntries}
                         hoverableLogEntries={!this.isTv}
                         scrollOnFocus={this.isTv}
                         onBack={window.serioController.back}
                         onLogEntry={entry => window.serioController.viewLogEntry(showCrawlerId, entry.id)}/>;
    }
    renderCrawlLogEntryDetails() {
        const {logEntry} = this.state.data;
        return <CrawlLogEntryDetails logEntry={logEntry}
                                     onBack={window.serioController.back}/>;
    }
    renderCrawlingInProgress() {
        return <CrawlingInProgress/>;
    }
    renderShowDetails() {
        const {show} = this.state.data;
        const id = show.id;
        return <ShowDetails show={show}
                            hoverableEpisodes={!this.isTv}
                            scrollOnFocus={this.isTv}
                            onPlay={() => window.serioController.playShow(id)}
                            onPlayEpisode={episode => window.serioController.playShowEpisode(id, episode.id)}
                            onBack={window.serioController.back}
                            onEdit={() => window.serioController.editShowCrawler(id)}
                            onShareCrawler={() => window.serioController.shareShowCrawler(id)}
                            onCrawl={() => window.serioController.crawlShow(id)}
                            onViewCrawlLog={() => window.serioController.viewShowCrawlLog(id)}
                            onClearWatchHistory={() => window.serioController.initiateClearShowWatchHistory(id)}
                            onDelete={() => window.serioController.initiateShowRemoval(id)}/>;
    }
    renderShowPlayer() {
        const {info, startProgress, showId} = this.state.data;
        return <ShowPlayer info={info}
                           startProgress={startProgress}
                           onProgressChange={progress => window.serioController.reportShowWatchProgress(showId, progress)}
                           onBack={window.serioController.back}
                           onEnd={() => window.serioController.playNextEpisode(showId)}
                           onPreviousEpisode={() => window.serioController.playPreviousEpisode(showId)}
                           onNextEpisode={() => window.serioController.playNextEpisode(showId)}/>;
    }
    renderClearWatchHistoryDialog() {
        const {showId, showName} = this.state.data;
        const actions = [
            {
                name: 'Cancel',
                isDefault: true,
                callback: () => window.serioController.back(),
            },
            {
                name: 'Confirm',
                callback: () => window.serioController.clearWatchHistory(showId),
            }
        ];
        return <Dialog title='Clear watch history'
                       text={`You are about to clear your watch history of '${showName}'.`}
                       actions={actions}/>
    }
    renderDeleteShowDialog() {
        const {showId, showName} = this.state.data;
        const actions = [
            {
                name: 'Cancel',
                isDefault: true,
                callback: () => window.serioController.back(),
            },
            {
                name: 'Confirm',
                callback: () => window.serioController.deleteShow(showId),
            }
        ];
        return <Dialog title='Delete show'
                       text={`You are about to delete '${showName}'.`}
                       actions={actions}/>;
    }
    renderWatchIsOverDialog() {
        const {showId, showName} = this.state.data;
        const actions = [
            {
                name: 'Watch another',
                isDefault: true,
                callback: () => window.serioController.viewAllShows(),
            },
            {
                name: 'Rewatch',
                callback: () => window.serioController.playShowFromTheBeginning(showId),
            }
        ];
        return <Dialog title={`'${showName}' is over`}
                       text={`You have watched all available episodes of '${showName}'.`}
                       actions={actions}/>;
    }
    renderErrorDialog() {
        const {errorMessage} = this.state.data;
        const actions = [
            {
                name: 'Understand',
                isDefault: true,
                callback: () => window.serioController.back(),
            }
        ];
        return <Dialog title='Whoops...' text={errorMessage} actions={actions}/>;
    }
}
