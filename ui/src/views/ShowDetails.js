import React from "react";
import './ShowDetails.css';
import {HEADLINE_5, HEADLINE_6, SUBTITLE_1, Text} from "../components/Text";
import Button from "../components/Button";
import {createScrollIntoTarget, getFunction} from "../common";
import ListTile from "../components/ListTile";
import {VISIBILITY} from "../components/Icons";

export default function ShowDetails(props) {
    const show = props.show;
    console.assert(show);
    const onPlay = getFunction(props.onPlay);
    const onPlayEpisode = getFunction(props.onPlayEpisode)
    const onBack = getFunction(props.onBack);
    const onEdit = getFunction(props.onEdit);
    const onShareCrawler = getFunction(props.onShareCrawler);
    const onCrawl = getFunction(props.onCrawl);
    const onViewCrawlLog = getFunction(props.onViewCrawlLog);
    const onClearWatchHistory = getFunction(props.onClearWatchHistory);
    const onDelete = getFunction(props.onDelete);
    const onFocusCapture = props.scrollOnFocus ? createScrollIntoTarget(false) : null;
    const backgroundStyle = {
        backgroundImage: `url(${show.thumbnail})`
    };
    let content;
    const hasEpisodes = show.episodes && show.episodes.length > 0;
    if (hasEpisodes) {
        const episodes = show.episodes.map(e =>
            <ListTile icon={e.watched ? VISIBILITY : undefined}
                      primaryText={e.name}
                      hoverable={props.hoverableEpisodes}
                      onFocusCapture={onFocusCapture}
                      secondaryText={e.videoUrl} onClick={() => onPlayEpisode(e, show)}/>
        );
        content = <div className='serio-vertical-container'>{episodes}</div>;
    } else {
        content = <div className='serio-show-details-no-episodes-placeholder serio-growable'><Text type={HEADLINE_6} primary>No episodes found =(</Text></div>;
    }
    return (
        <div className='serio-show-details serio-full-height' style={backgroundStyle}>
            <div className='serio-show-details-container serio-full-height'>
                <div className='serio-padding serio-show-details-title'>
                    <Text type={HEADLINE_5} primary>{show.name}</Text>
                    <Text type={SUBTITLE_1}>Last watched {show.lastWatched}</Text>
                </div>
                <div>
                    <Button text='play' autoFocus={hasEpisodes} className='serio-margin' onClick={() => onPlay(show)} disabled={!hasEpisodes}/>
                    <Button text='back' className='serio-margin' onClick={onBack}/>
                    <Button text='edit' className='serio-margin' onClick={() => onEdit(show)} autoFocus={!hasEpisodes}/>
                    <Button text='share crawler' className='serio-margin' onClick={() => onShareCrawler(show)}/>
                    <Button text='crawl' className='serio-margin' onClick={() => onCrawl(show)}/>
                    <Button text='view crawl log' className='serio-margin' onClick={() => onViewCrawlLog(show)}/>
                    <Button text='clear watch history' className='serio-margin' onClick={() => onClearWatchHistory(show)}/>
                    <Button text='delete' className='serio-margin' onClick={() => onDelete(show)}/>
                </div>
                {content}
            </div>
        </div>
    );
}
