import React from "react";
import './ImportShowCrawler.css';
import {getFunction} from "../common";
import {HEADLINE_5, Text} from "../components/Text";
import TextArea from "../components/TextArea";
import Button from "../components/Button";

export default function ImportShowCrawler(props) {
    const onCancel = getFunction(props.onCancel);
    const onImport = getFunction(props.onImport);
    const onChange = getFunction(props.onShowCrawlerChange);
    return (
        <div className='serio-import-show-crawler serio-full-height'>
            <div className='serio-margin'>
                <Text type={HEADLINE_5} primary>Import show crawler</Text>
            </div>
            <TextArea className='serio-margin serio-growable'
                      placeholder={'Paste configuration of a show crawler here'}
                      defaultValue={props.showCrawler}
                      onChange={e => onChange(e.target.value)}
                      autoFocus={true}/>
            <div className='serio-form-actions serio-margin'>
                <Button text='cancel' onClick={onCancel}/>
                <Button text='save' onClick={onImport} className='serio-margin-before'/>
            </div>
        </div>
    );
}
