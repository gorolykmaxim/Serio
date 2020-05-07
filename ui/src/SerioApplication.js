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
import CrawlerHelp from "./views/CrawlerHelp";

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
export const SHOW_OVERRIDE_DIALOG = 14;
export const CRAWLER_HELP = 15;

export class SerioApplication extends React.Component {
    constructor(props, context) {
        super(props, context);
        this.state = {data: {}};
        this.handleEvent = this.handleEvent.bind(this);
    }
    get isTv() {
        return this.props.isTv;
    }
    get isDesktop() {
        return this.props.isDesktop;
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
            case SHOW_OVERRIDE_DIALOG:
                return this.renderShowOverrideDialog();
            case CRAWLER_HELP:
                return this.renderCrawlerHelp();
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
                         onAddShow={() => window.serioController.addShow()}
                         onImportShowFromJson={() => window.serioController.importShowFromJson()}
                         onShowSelect={(show) => window.serioController.selectShow(show.id)}/>;
    }
    renderEditShowCrawler() {
        const {showId, showName} = this.state.data;
        return <EditShowCrawler showId={showId}
                                showName={showName}
                                onShowNameChange={showName => window.serioController.setShowName(showName)}
                                onEditThumbnailCrawler={() => window.serioController.editThumbnailCrawler()}
                                onEditEpisodeVideoCrawler={() => window.serioController.editEpisodeVideoCrawler()}
                                onEditEpisodeNameCrawler={() => window.serioController.editEpisodeNameCrawler()}
                                onCancel={() => window.serioController.back()}
                                onSave={() => window.serioController.saveShowCrawler()}/>;
    }
    renderImportShowCrawler() {
        const {showCrawler} = this.state.data;
        return <ImportShowCrawler showCrawler={showCrawler}
                                  onImport={() => window.serioController.importShowCrawler()}
                                  onShowCrawlerChange={showCrawler => window.serioController.setShowCrawler(showCrawler)}
                                  onCancel={() => window.serioController.back()}/>;
    }
    renderEditCrawler() {
        const {crawlerType, crawler} = this.state.data;
        return <EditCrawler crawlerType={crawlerType}
                            crawler={crawler}
                            onCrawlerChange={rawCrawler => window.serioController.setCrawler(rawCrawler)}
                            onCancel={() => window.serioController.back()}
                            onHelp={() => window.serioController.displayCrawlerHelp()}
                            onPreview={() => window.serioController.previewCrawler()}
                            onSave={() => window.serioController.saveCrawler()}/>;
    }
    renderCrawlPreview() {
        const {crawlerType, crawlItems} = this.state.data;
        return <CrawlPreview crawlerType={crawlerType}
                             crawlItems={crawlItems}
                             hoverableCrawlItems={!this.isTv}
                             scrollOnFocus={this.isTv}
                             onBack={() => window.serioController.back()}
                             onViewLog={() => window.serioController.viewCrawlLog()}/>;
    }
    renderCrawlLog() {
        const {crawlerType, logEntries} = this.state.data;
        return <CrawlLog crawlerType={crawlerType}
                         logEntries={logEntries}
                         hoverableLogEntries={!this.isTv}
                         scrollOnFocus={this.isTv}
                         onBack={() => window.serioController.back()}
                         onLogEntry={entry => window.serioController.viewLogEntry(entry.id)}/>;
    }
    renderCrawlLogEntryDetails() {
        const {logEntry} = this.state.data;
        return <CrawlLogEntryDetails logEntry={logEntry}
                                     onBack={() => window.serioController.back()}/>;
    }
    renderCrawlingInProgress() {
        return <CrawlingInProgress/>;
    }
    renderShowDetails() {
        const {show} = this.state.data;
        return <ShowDetails show={show}
                            hoverableEpisodes={!this.isTv}
                            scrollOnFocus={this.isTv}
                            onPlay={() => window.serioController.playShow()}
                            onPlayEpisode={episode => window.serioController.playShowEpisode(episode.id)}
                            onBack={() => window.serioController.back()}
                            onEdit={() => window.serioController.editShowCrawler()}
                            onShareCrawler={() => window.serioController.shareShowCrawler()}
                            onCrawl={() => window.serioController.crawlShow()}
                            onViewCrawlLog={() => window.serioController.viewShowCrawlLog()}
                            onClearWatchHistory={() => window.serioController.initiateClearShowWatchHistory()}
                            onDelete={() => window.serioController.initiateShowRemoval()}/>;
    }
    renderShowPlayer() {
        const {videoUrl, showName, episodeName, hasPreviousEpisode, hasNextEpisode, startProgress} = this.state.data;
        return <ShowPlayer videoUrl={videoUrl}
                           showName={showName}
                           episodeName={episodeName}
                           hasPreviousEpisode={hasPreviousEpisode}
                           hasNextEpisode={hasNextEpisode}
                           startProgress={startProgress}
                           fullBodyPlayPause={this.isDesktop}
                           onProgressChange={progress => window.serioController.reportShowWatchProgress(progress)}
                           onBack={() => window.serioController.back()}
                           onEnd={() => window.serioController.playNextEpisode()}
                           onError={error => window.serioController.displayError(`${error}. You might need to re-crawl the show.`)}
                           onPreviousEpisode={() => window.serioController.playPreviousEpisode()}
                           onNextEpisode={() => window.serioController.playNextEpisode()}/>;
    }
    renderClearWatchHistoryDialog() {
        const {showName} = this.state.data;
        const actions = [
            {
                name: 'Cancel',
                isDefault: true,
                callback: () => window.serioController.back(),
            },
            {
                name: 'Confirm',
                callback: () => window.serioController.clearWatchHistory(),
            }
        ];
        return <Dialog title='Clear watch history'
                       text={`You are about to clear your watch history of '${showName}'.`}
                       actions={actions}/>
    }
    renderDeleteShowDialog() {
        const {showName} = this.state.data;
        const actions = [
            {
                name: 'Cancel',
                isDefault: true,
                callback: () => window.serioController.back(),
            },
            {
                name: 'Confirm',
                callback: () => window.serioController.deleteShow(),
            }
        ];
        return <Dialog title='Delete show'
                       text={`You are about to delete '${showName}'.`}
                       actions={actions}/>;
    }
    renderWatchIsOverDialog() {
        const {showName} = this.state.data;
        const actions = [
            {
                name: 'Watch another',
                isDefault: true,
                callback: () => window.serioController.viewAllShows(),
            },
            {
                name: 'Rewatch',
                callback: () => window.serioController.playShowFromTheBeginning(),
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
    renderShowOverrideDialog() {
        const {showName} = this.state.data;
        const actions = [
            {
                name: 'Cancel',
                isDefault: true,
                callback: () => window.serioController.back()
            },
            {
                name: 'Confirm',
                callback: () => window.serioController.confirmShowOverride()
            }
        ];
        return <Dialog title={`Show '${showName}' already exists`}
                       text={'Are you sure you want to override the existing crawler and the show with the new ones?'}
                       actions={actions}/>;
    }
    renderCrawlerHelp() {
        return <CrawlerHelp onBack={() => window.serioController.back()}/>;
    }
}
