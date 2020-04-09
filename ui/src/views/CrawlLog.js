import React from "react";
import './CrawlLog.css';
import IconButton from "../components/IconButton";
import {createScrollIntoTarget, getFunction} from "../common";
import {HEADLINE_5, Text} from "../components/Text";
import ListTile from "../components/ListTile";

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
            <div className='serio-crawl-log-title serio-padding'>
                <IconButton icon='arrow_back' autoFocus={true} onClick={onBack}/>
                <Text type={HEADLINE_5} primary className='serio-margin-before'>{props.crawlType ? `${props.crawlType} crawl log` : 'Crawl log'}</Text>
            </div>
            <div className='serio-growable serio-vertical-container'>
                {logEntries}
            </div>
        </div>
    );
}
