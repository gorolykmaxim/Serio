import React from "react";
import './ImportShowCrawler.css';
import {getFunction} from "../common";
import {HEADLINE_5, Text} from "../components/Text";
import TextArea from "../components/TextArea";
import Button from "../components/Button";

export default function ImportShowCrawler(props) {
    const onCancel = getFunction(props.onCancel);
    const onImport = getFunction(props.onImport);
    return (
        <form className='serio-import-show-crawler' onSubmit={createOnSubmit(onImport)}>
            <div className='serio-margin'>
                <Text type={HEADLINE_5} primary>Import show crawler</Text>
            </div>
            <TextArea className='serio-margin serio-import-show-crawler-text-area'
                      required={true}
                      placeholder={'Paste configuration of a show crawler here'}
                      name={'showCrawler'}
                      autoFocus={true}/>
            <div className='serio-import-show-crawler-actions serio-margin'>
                <Button text='cancel' onClick={onCancel}/>
                <Button text='save' submit={true} className='serio-margin-before'/>
            </div>
        </form>
    );
}

function createOnSubmit(callback) {
    return event => {
        event.preventDefault();
        callback(event.target.elements.showCrawler.value);
    };
}
