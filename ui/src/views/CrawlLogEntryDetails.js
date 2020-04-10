import React from "react";
import './CrawlLogEntryDetails.css';
import {BODY_1, HEADLINE_5, Text} from "../components/Text";
import {getFunction} from "../common";
import Button from "../components/Button";

export default function CrawlLogEntryDetails(props) {
    console.assert(props.logEntry);
    const logEntry = props.logEntry;
    const onBack = getFunction(props.onBack);
    return (
        <div className='serio-crawl-log-entry-details serio-full-height'>
            <div className='serio-padding'>
                <Text type={HEADLINE_5} primary>Crawl log entry details</Text>
            </div>
            <div className='serio-padding'>
                <Button text='back' onClick={onBack} autoFocus={true}/>
            </div>
            <div className='serio-padding serio-crawl-log-entry-details-multiline-text'>
                <Text wrap={true} type={BODY_1}>{logEntry.description}</Text>
            </div>
            <div className='serio-padding serio-crawl-log-entry-details-multiline-text'>
                <Text wrap={true} type={BODY_1}>{logEntry.input}</Text>
            </div>
            <div className='serio-padding serio-crawl-log-entry-details-multiline-text'>
                <Text wrap={true} type={BODY_1}>{logEntry.output}</Text>
            </div>
        </div>
    );
}
