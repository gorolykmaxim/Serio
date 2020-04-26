import React from "react";
import './EditShowCrawler.css';
import TextEdit from "../components/TextEdit";
import Button from "../components/Button";
import {HEADLINE_5, SUBTITLE_2, Text} from "../components/Text";
import {getFunction} from "../common";

export default function EditShowCrawler(props) {
    const isNewCrawler = !props.showId;
    const onEditThumbnailCrawler = getFunction(props.onEditThumbnailCrawler);
    const onEditEpisodeVideoCrawler = getFunction(props.onEditEpisodeVideoCrawler);
    const onEditEpisodeNameCrawler = getFunction(props.onEditEpisodeNameCrawler);
    const onCancel = getFunction(props.onCancel);
    const onSave = getFunction(props.onSave);
    return (
        <div className='serio-edit-show-crawler serio-full-height'>
            <form className='serio-edit-show-crawler-container' onSubmit={createOnSubmit(onSave)}>
                <div>
                    <Text type={HEADLINE_5} primary={true}>Configure show crawler</Text>
                </div>
                <TextEdit name={'name'}
                          required={true}
                          placeholder={'TV Show name'}
                          defaultValue={props.showName}
                          autoFocus={isNewCrawler}
                          disabled={!isNewCrawler}
                          className='serio-edit-show-crawler-edit-line serio-growable'/>
                <Button text='Configure episode video crawler'
                        className='serio-edit-show-crawler-edit-line serio-growable'
                        onClick={onEditEpisodeVideoCrawler}/>
                <Text type={SUBTITLE_2} primary>Optional:</Text>
                <Button text='Configure thumbnail crawler'
                        className='serio-edit-show-crawler-edit-line serio-growable'
                        onClick={onEditThumbnailCrawler}/>
                <Button text='Configure episode name crawler'
                        className='serio-edit-show-crawler-edit-line serio-growable'
                        onClick={onEditEpisodeNameCrawler}/>
                <div className='serio-edit-show-crawler-edit-line serio-growable serio-form-actions'>
                    <Button text={'cancel'} autoFocus={!isNewCrawler} onClick={onCancel}/>
                    <Button submit={true} text={'save'} className='serio-margin-before'/>
                </div>
            </form>
        </div>
    );
}

function createOnSubmit(callback) {
    return (event) => {
        event.preventDefault();
        callback(event.target.elements.name.value);
    };
}
