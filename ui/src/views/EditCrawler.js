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
    const onPreview = getFunction(props.onPreview);
    const onSave = getFunction(props.onSave);
    return (
        <form className='serio-edit-crawler serio-full-height' onSubmit={createOnSubmit(onPreview, onSave)}>
            <div className='serio-margin'>
                <Text type={HEADLINE_5} primary>Configure {crawlerType} crawler</Text>
            </div>
            <TextArea className='serio-margin serio-growable'
                      required={true}
                      placeholder='Enter configuration of the crawler'
                      name={'crawler'}
                      defaultValue={crawler}
                      autoFocus={true}/>
            <input type={'hidden'} name={'isPreview'}/>
            <div className='serio-form-actions serio-margin'>
                <Button text={'cancel'} onClick={onCancel}/>
                <Button text={'preview'} onClick={createOnClick(true)} className='serio-margin-before'/>
                <Button text={'save'} onClick={createOnClick(false)} className='serio-margin-before'/>
            </div>
        </form>
    );
}

function createOnClick(isPreview) {
    return event => {
        const form = event.target.parentElement.parentElement;
        form.isPreview.value = isPreview;
        if (form.reportValidity()) {
            form.dispatchEvent(new Event('submit', {bubbles: true, cancelable: true}));
        }
    }
}

function createOnSubmit(onPreview, onSave) {
    return event => {
        event.preventDefault();
        const isPreview = event.target.isPreview.value === 'true';
        const crawler = event.target.crawler.value;
        if (isPreview) {
            onPreview(crawler);
        } else {
            onSave(crawler);
        }
    };
}
