import React from "react";
import './AllShows.css';
import Button from "../components/Button";
import ShowCard from "../components/ShowCard";
import {HEADLINE_5, Text} from "../components/Text";

export default function AllShows(props) {
    const lastWatchedShows = props.showInfo.lastWatchedShows || [];
    const allShows = props.showInfo.allShows || [];
    const onShowSelect = props.onShowSelect || (() => {});
    const content = [];
    const hasWatchedShows = lastWatchedShows.length > 0;
    if (hasWatchedShows) {
        content.push(
            <div className='serio-padding'>
                <Text type={HEADLINE_5} primary>Last Watched</Text>
            </div>
        );
        const shows = [];
        for (let i = 0; i < lastWatchedShows.length; i++) {
            shows.push(
                <ShowCard show={lastWatchedShows[i]}
                          autoFocus={i === 0}
                          className='serio-margin-before'
                          onClick={onShowSelect}/>
            );
        }
        content.push(
            <div className='serio-all-shows-growable serio-horizontal-container'>{shows}</div>
        );
    }
    content.push(
        <div className='serio-padding'>
            <Button text='add show' autoFocus={!hasWatchedShows}/>
            <Button className='serio-margin-before' text='import show from json'/>
        </div>
    );
    content.push(
        <div className='serio-all-shows-growable serio-horizontal-container'>
            {allShows.map(s => (
                <ShowCard show={s}
                          className='serio-margin-before'
                          onClick={onShowSelect}/>
            ))}
        </div>
    );
    return <div className='serio-all-shows'>{content}</div>;
}
