import React from "react";
import './EditCrawler.css';
import {getFunction} from "../common";
import {HEADLINE_5, Text} from "../components/Text";
import TextArea from "../components/TextArea";
import Button from "../components/Button";

export default function EditCrawler(props) {
    const crawlerType = props.crawlerType;
    console.assert(crawlerType);
    const crawler = props.crawler;
    const onCancel = getFunction(props.onCancel);
    const onCrawlerChange = getFunction(props.onCrawlerChange);
    const onPreview = getFunction(props.onPreview);
    const onSave = getFunction(props.onSave);
    return (
        <div className='serio-edit-crawler serio-full-height'>
            <div className='serio-margin'>
                <Text type={HEADLINE_5} primary>Configure {crawlerType} crawler</Text>
            </div>
            <TextArea className='serio-margin serio-growable'
                      placeholder='Enter configuration of the crawler'
                      onChange={e => onCrawlerChange(e.target.value)}
                      defaultValue={crawler}
                      autoFocus={true}/>
            <div className='serio-form-actions serio-margin'>
                <Button text={'cancel'} onClick={onCancel}/>
                <Button text={'preview'} onClick={onPreview} className='serio-margin-before'/>
                <Button text={'save'} onClick={onSave} className='serio-margin-before'/>
            </div>
        </div>
    );
}
