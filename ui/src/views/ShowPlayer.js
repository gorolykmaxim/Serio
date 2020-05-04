import React from "react";
import './ShowPlayer.css';
import ReactPlayer from "react-player";
import format from 'format-duration';
import {Line} from "rc-progress";
import {HEADLINE_5, SUBTITLE_2, Text} from "../components/Text";
import {IconButton, MEDIUM_SIZE} from "../components/IconButton";
import {getFunction} from "../common";
import CircularProgress from "../components/CircularProgress";
import {
    ARROW_BACK,
    FORWARD_30,
    FULLSCREEN,
    FULLSCREEN_EXIT,
    PAUSE,
    PLAY_ARROW,
    REPLAY_3O,
    SKIP_NEXT,
    SKIP_PREVIOUS
} from "../components/Icons";

const screenfull = require('screenfull');

const SEEK_TIME_PERIOD = 30;
const PROGRESS_INTERVAL = 10;
const INACTIVITY_TIMEOUT = 5000;

export default class ShowPlayer extends React.Component {
    constructor(props, context) {
        super(props, context);
        console.assert(props.videoUrl);
        console.assert(props.showName);
        console.assert(props.episodeName);
        this.inactivityTimeout = null;
        this.player = null;
        this.isInitialBufferingComplete = false;
        this.state = {
            playedTime: 0,
            totalTime: 0,
            playedPercent: 0,
            isPlaying: true,
            isFullScreen: false,
            isBuffering: true,
            displayControls: true
        };
    }
    get progressInterval() {
        return this.props.progressInterval || PROGRESS_INTERVAL;
    }
    get startProgress() {
        return this.props.startProgress;
    }
    get onBack() {
        return getFunction(this.props.onBack);
    }
    get onPreviousEpisode() {
        return getFunction(this.props.onPreviousEpisode);
    }
    get onNextEpisode() {
        return getFunction(this.props.onNextEpisode);
    }
    get onProgressChange() {
        return getFunction(this.props.onProgressChange);
    }
    get onEnd() {
        return getFunction(this.props.onEnd);
    }
    get onError() {
        return getFunction(this.props.onError);
    }
    componentDidMount() {
        this.handleUserActivity();
    }
    componentWillUnmount() {
        clearTimeout(this.inactivityTimeout);
    }
    togglePlay() {
        this.setState({isPlaying: !this.state.isPlaying});
    }
    createSeek(timeDelta) {
        return () => {
            this.player.seekTo(this.state.playedTime + timeDelta);
        }
    }
    onPlay() {
        this.setState({isPlaying: true});
    }
    onBuffer() {
        this.setState({isBuffering: true});
    }
    onBufferEnd() {
        if (!this.isInitialBufferingComplete && this.startProgress) {
            this.player.seekTo(this.startProgress / 100);
        }
        this.isInitialBufferingComplete = true;
        this.setState({isBuffering: false});
    }
    onPause() {
        this.setState({isPlaying: false});
    }
    onProgress(progress) {
        this.handleProgress(Math.round(progress.playedSeconds));
    }
    async toggleFullScreen() {
        await screenfull.toggle();
        this.setState({isFullScreen: !this.state.isFullScreen});
    }
    handleProgress(playedTime) {
        const totalTime = this.state.totalTime;
        const playedPercent = totalTime > 0 ? playedTime / totalTime * 100 : 0;
        if (playedTime % this.progressInterval === 0) {
            this.onProgressChange(playedPercent);
        }
        this.setState({
            playedTime: playedTime,
            playedPercent: playedPercent
        });
    }
    onDuration(duration) {
        this.setState({totalTime: Math.round(duration)});
    }
    handleUserActivity() {
        if (this.inactivityTimeout !== null) {
            clearTimeout(this.inactivityTimeout);
        } else {
            this.setState({displayControls: true});
        }
        this.inactivityTimeout = setTimeout(() => {
            this.setState({displayControls: false});
            this.inactivityTimeout = null;
        }, INACTIVITY_TIMEOUT);
    }
    render() {
        const {playedTime, totalTime, playedPercent, isPlaying, isBuffering, displayControls, isFullScreen} = this.state;
        const {videoUrl, showName, episodeName, hasPreviousEpisode, hasNextEpisode, enableFullScreen} = this.props;
        const controlsStyle = {opacity: displayControls ? 1 : 0, cursor: displayControls ? 'auto' : 'none'};
        const buffering = isBuffering ? <CircularProgress/> : null;
        let fullscreenButton = null;
        if (enableFullScreen) {
            fullscreenButton = <IconButton icon={isFullScreen ? FULLSCREEN_EXIT : FULLSCREEN}
                                           size={MEDIUM_SIZE}
                                           onClickStopPropagate={this.toggleFullScreen.bind(this)}/>;
        }
        return (
            <div className='serio-full-height serio-show-player-container' onMouseMove={this.handleUserActivity.bind(this)} onKeyDown={this.handleUserActivity.bind(this)}>
                <ReactPlayer width='100%'
                             height='100%'
                             url={videoUrl}
                             ref={player => {this.player = player}}
                             onPlay={this.onPlay.bind(this)}
                             onPause={this.onPause.bind(this)}
                             onEnded={this.onEnd.bind(this)}
                             onProgress={this.onProgress.bind(this)}
                             onSeek={this.handleProgress.bind(this)}
                             onDuration={this.onDuration.bind(this)}
                             onBuffer={this.onBuffer.bind(this)}
                             onBufferEnd={this.onBufferEnd.bind(this)}
                             onError={() => this.onError(`Can't play ${videoUrl}`)}
                             playing={isPlaying}/>
                <div className='serio-show-player-layer serio-show-player-buffering'>
                    {buffering}
                </div>
                <div className='serio-show-player-layer serio-show-player-controls' style={controlsStyle} onClick={this.togglePlay.bind(this)}>
                    <div className='serio-show-player-title serio-show-player-margin'>
                        <IconButton icon={ARROW_BACK}
                                    size={MEDIUM_SIZE}
                                    onClickStopPropagate={this.onBack}
                                    className='serio-show-player-margin-after'/>
                        <Text type={HEADLINE_5} primary>{showName}</Text>
                    </div>
                    <div className='serio-show-player-navigation'>
                        <div className='serio-show-player-progress'>
                            <Text type={SUBTITLE_2} className='serio-show-player-horizontal-margin'>{format(playedTime * 1000)}</Text>
                            <div className='serio-growable'>
                                <Line percent={playedPercent} style={{height: '8px', width: '100%'}} strokeColor='#FB8C00'/>
                            </div>
                            <Text type={SUBTITLE_2} className='serio-show-player-horizontal-margin'>{format(totalTime * 1000)}</Text>
                        </div>
                        <div className='serio-show-player-episode-navigation serio-show-player-padding'>
                            <IconButton icon={isPlaying ? PAUSE : PLAY_ARROW}
                                        className='serio-show-player-margin-after'
                                        size={MEDIUM_SIZE}
                                        autoFocus={true}
                                        onClickStopPropagate={this.togglePlay.bind(this)}/>
                            <IconButton icon={REPLAY_3O}
                                        className='serio-show-player-margin-after'
                                        size={MEDIUM_SIZE}
                                        onClickStopPropagate={this.createSeek(SEEK_TIME_PERIOD * -1)}/>
                            <IconButton icon={FORWARD_30}
                                        className='serio-show-player-margin-after'
                                        size={MEDIUM_SIZE}
                                        onClickStopPropagate={this.createSeek(SEEK_TIME_PERIOD)}/>
                            <IconButton icon={SKIP_PREVIOUS}
                                        className='serio-show-player-margin-after'
                                        size={MEDIUM_SIZE}
                                        isDisabled={!hasPreviousEpisode}
                                        onClickStopPropagate={this.onPreviousEpisode}/>
                            <Text type={HEADLINE_5} className='serio-show-player-margin-after' primary>{episodeName}</Text>
                            <IconButton icon={SKIP_NEXT}
                                        className='serio-show-player-margin-after'
                                        size={MEDIUM_SIZE}
                                        isDisabled={!hasNextEpisode}
                                        onClickStopPropagate={this.onNextEpisode}/>
                            <span className='serio-growable'/>
                            {fullscreenButton}
                        </div>
                    </div>
                </div>
            </div>
        );
    }
}
