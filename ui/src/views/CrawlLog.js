import React from "react";
import './CrawlLog.css';
import {createScrollIntoTarget, getFunction} from "../common";
import {HEADLINE_5, Text} from "../components/Text";
import ListTile from "../components/ListTile";
import Button from "../components/Button";

export default function CrawlLog(props) {
    console.assert(props.logEntries);
    const onBack = getFunction(props.onBack);
    const onLogEntry = getFunction(props.onLogEntry);
    const onFocusCapture = props.scrollOnFocus ? createScrollIntoTarget(false) : null;
    const logEntries = props.logEntries.map(e =>
        <ListTile primaryText={e.name}
                  secondaryText={e.description}
                  hoverable={props.hoverableLogEntries}
                  onClick={() => onLogEntry(e)}
                  onFocusCapture={onFocusCapture}/>
    );
    return (
        <div className='serio-crawl-log serio-full-height'>
            <div className='serio-padding'>
                <Text type={HEADLINE_5} primary>{props.crawlType ? `${props.crawlType} crawl log` : 'Crawl log'}</Text>
            </div>
            <div className='serio-padding'>
                <Button text='back' onClick={onBack} autoFocus={true}/>
            </div>
            <div className='serio-growable serio-vertical-container'>
                {logEntries}
            </div>
        </div>
    );
}
