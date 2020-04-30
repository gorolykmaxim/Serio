import React from "react";
import './CrawlPreview.css';
import {HEADLINE_5, Text} from "../components/Text";
import Button from "../components/Button";
import ListTile from "../components/ListTile";
import {createScrollIntoTarget, getFunction} from "../common";
const capitalize = require('capitalize');

export default function CrawlPreview(props) {
    console.assert(props.crawlerType);
    console.assert(props.crawlItems);
    const onBack = getFunction(props.onBack);
    const onViewLog = getFunction(props.onViewLog);
    const onFocusCapture = props.scrollOnFocus ? createScrollIntoTarget(false): null;
    const crawlItems = props.crawlItems.map(i => <ListTile icon={null} primaryText={i} hoverable={props.hoverableCrawlItems} onFocusCapture={onFocusCapture}/>);
    const title = capitalize(`${props.crawlerType} crawling preview`);
    return (
        <div className='serio-crawl-preview serio-full-height'>
            <div className='serio-padding'>
                <Text type={HEADLINE_5} primary>{title}</Text>
            </div>
            <div className='serio-actions serio-padding'>
                <Button text='back' className='serio-margin-after' onClick={onBack} autoFocus={true}/>
                <Button text='view log' onClick={onViewLog}/>
            </div>
            <div className='serio-growable serio-vertical-container'>
                {crawlItems}
            </div>
        </div>
    );
}
