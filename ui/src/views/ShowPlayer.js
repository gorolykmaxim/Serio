import React from "react";
import './ShowPlayer.css';
import ReactPlayer from "react-player";
import format from 'format-duration';
import {Line} from "rc-progress";
import {HEADLINE_5, SUBTITLE_2, Text} from "../components/Text";
import {IconButton, MEDIUM_SIZE} from "../components/IconButton";
import {getFunction} from "../common";
import CircularProgress from "../components/CircularProgress";

const SEEK_TIME_PERIOD = 30;
const PROGRESS_INTERVAL = 10;
const INACTIVITY_TIMEOUT = 5000;

export default class ShowPlayer extends React.Component {
    constructor(props, context) {
        super(props, context);
        console.assert(this.info);
        this.inactivityTimeout = null;
        this.player = null;
        this.state = {
            playedTime: 0,
            totalTime: 0,
            playedPercent: 0,
            isPlaying: true,
            isBuffering: true,
            displayControls: true
        };
    }
    get info() {
        return this.props.info;
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
    onStart() {
        if (this.startProgress) {
            this.player.seekTo(this.startProgress / 100);
        }
    }
    onPlay() {
        this.setState({isPlaying: true});
    }
    onBuffer() {
        this.setState({isBuffering: true});
    }
    onBufferEnd() {
        this.setState({isBuffering: false});
    }
    onPause() {
        this.setState({isPlaying: false});
    }
    onProgress(progress) {
        this.handleProgress(Math.round(progress.playedSeconds));
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
        const {playedTime, totalTime, playedPercent, isPlaying, isBuffering, displayControls} = this.state;
        const controlsStyle = {opacity: displayControls ? 1 : 0};
        const buffering = isBuffering ? <CircularProgress/> : null;
        return (
            <div className='serio-full-height' onMouseMove={this.handleUserActivity.bind(this)} onKeyDown={this.handleUserActivity.bind(this)}>
                <ReactPlayer width='100%'
                             height='100%'
                             url={this.info.videoUrl}
                             ref={player => {this.player = player}}
                             onStart={this.onStart.bind(this)}
                             onPlay={this.onPlay.bind(this)}
                             onPause={this.onPause.bind(this)}
                             onEnded={this.onEnd.bind(this)}
                             onProgress={this.onProgress.bind(this)}
                             onSeek={this.handleProgress.bind(this)}
                             onDuration={this.onDuration.bind(this)}
                             onBuffer={this.onBuffer.bind(this)}
                             onBufferEnd={this.onBufferEnd.bind(this)}
                             playing={isPlaying}/>
                <div className='serio-show-player-layer serio-show-player-buffering'>
                    {buffering}
                </div>
                <div className='serio-show-player-layer serio-show-player-controls' style={controlsStyle}>
                    <div className='serio-show-player-title serio-show-player-margin'>
                        <IconButton icon='arrow_back'
                                    size={MEDIUM_SIZE}
                                    onClick={this.onBack}
                                    className='serio-show-player-margin-after'/>
                        <Text type={HEADLINE_5} primary>{this.info.showName}</Text>
                    </div>
                    <div className='serio-show-player-navigation'>
                        <div className='serio-show-player-progress'>
                            <Text type={SUBTITLE_2} className='serio-show-player-horizontal-margin'>{format(playedTime * 1000)}</Text>
                            <div className='serio-growable'>
                                <Line percent={playedPercent} strokeColor='#FB8C00'/>
                            </div>
                            <Text type={SUBTITLE_2} className='serio-show-player-horizontal-margin'>{format(totalTime * 1000)}</Text>
                        </div>
                        <div className='serio-show-player-episode-navigation serio-show-player-padding'>
                            <IconButton icon={isPlaying ? 'pause' : 'play_arrow'}
                                        className='serio-show-player-margin-after'
                                        size={MEDIUM_SIZE}
                                        autoFocus={true}
                                        onClick={this.togglePlay.bind(this)}/>
                            <IconButton icon='replay_30'
                                        className='serio-show-player-margin-after'
                                        size={MEDIUM_SIZE}
                                        onClick={this.createSeek(SEEK_TIME_PERIOD * -1)}/>
                            <IconButton icon='forward_30'
                                        className='serio-show-player-margin-after'
                                        size={MEDIUM_SIZE}
                                        onClick={this.createSeek(SEEK_TIME_PERIOD)}/>
                            <IconButton icon='skip_previous'
                                        className='serio-show-player-margin-after'
                                        size={MEDIUM_SIZE}
                                        isDisabled={!this.info.hasPreviousEpisode}
                                        onClick={this.onPreviousEpisode}/>
                            <Text type={HEADLINE_5} className='serio-show-player-margin-after' primary>{this.info.episodeName}</Text>
                            <IconButton icon='skip_next'
                                        className='serio-show-player-margin-after'
                                        size={MEDIUM_SIZE}
                                        isDisabled={!this.info.hasNextEpisode}
                                        onClick={this.onNextEpisode}/>
                        </div>
                    </div>
                </div>
            </div>
        );
    }
}
