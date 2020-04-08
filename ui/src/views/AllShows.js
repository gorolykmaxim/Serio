import React from "react";
import './AllShows.css';
import Button from "../components/Button";
import ShowCard from "../components/ShowCard";
import {HEADLINE_5, Text} from "../components/Text";
import {getFunction, scrollIntoTarget} from "../common";

export default function AllShows(props) {
    const lastWatchedShows = props.showInfo.lastWatchedShows || [];
    const allShows = props.showInfo.allShows || [];
    const onShowSelect = getFunction(props.onShowSelect);
    const onAddShow = getFunction(props.onAddShow);
    const onImportShowFromJson = getFunction(props.onImportShowFromJson);
    const hoverableShowCards = props.hoverableShowCards || false;
    const onFocusCapture = props.scrollOnFocus ? scrollIntoTarget : null;
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
                          hoverable={hoverableShowCards}
                          onFocusCapture={onFocusCapture}
                          className='serio-margin-after'
                          onClick={onShowSelect}/>
            );
        }
        content.push(
            <div className='serio-all-shows-growable serio-horizontal-container serio-margin'>{shows}</div>
        );
    }
    content.push(
        <div className='serio-padding'>
            <Button text='add show' autoFocus={!hasWatchedShows} onClick={onAddShow}/>
            <Button className='serio-margin-before' text='import show from json' onClick={onImportShowFromJson}/>
        </div>
    );
    content.push(
        <div className='serio-all-shows-growable serio-horizontal-container serio-margin'>
            {allShows.map(s => (
                <ShowCard show={s}
                          className='serio-margin-after'
                          hoverable={hoverableShowCards}
                          onFocusCapture={onFocusCapture}
                          onClick={onShowSelect}/>
            ))}
        </div>
    );
    return <div className='serio-all-shows'>{content}</div>;
}
