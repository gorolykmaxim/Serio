import React from "react";
import './CrawlLog.css';
import {createScrollIntoTarget, getFunction} from "../common";
import {HEADLINE_5, Text} from "../components/Text";
import ListTile from "../components/ListTile";
import Button from "../components/Button";
const capitalize = require('capitalize');

export default function CrawlLog(props) {
    console.assert(props.logEntries);
    const onBack = getFunction(props.onBack);
    const onLogEntry = getFunction(props.onLogEntry);
    const onFocusCapture = props.scrollOnFocus ? createScrollIntoTarget(false) : null;
    const logEntries = props.logEntries.map(e =>
        <ListTile primaryText={e.description}
                  icon={null}
                  hoverable={props.hoverableLogEntries}
                  onClick={e.hasDetails ? () => onLogEntry(e) : null}
                  onFocusCapture={onFocusCapture}/>
    );
    const title = capitalize(props.crawlerType ? `${props.crawlerType} crawl log` : 'Crawl log');
    return (
        <div className='serio-crawl-log serio-full-height'>
            <div className='serio-padding'>
                <Text type={HEADLINE_5} primary>{title}</Text>
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
