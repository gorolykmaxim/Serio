import React from "react";
import './ShowCard.css';
import {HEADLINE_5, SUBTITLE_1, Text} from "./Text";
import {getFunction} from "../common";

export default function ShowCard(props) {
    const {name, thumbnail, lastWatched} = props.show;
    const backgroundStyle = {backgroundImage: `url(${thumbnail})`};
    const onClick = getFunction(props.onClick);
    const className = ['serio-show-card'];
    if (props.hoverable) {
        className.push('serio-show-card-hoverable');
    }
    if (props.className) {
        className.push(props.className);
    }
    return (
        <button className={className.join(' ')}
                style={backgroundStyle}
                onFocusCapture={props.onFocusCapture}
                autoFocus={props.autoFocus}
                onClick={() => onClick(props.show)}>
            <div className='serio-show-card-inside-container'>
                <Text type={HEADLINE_5}>{name}</Text>
                <Text type={SUBTITLE_1}>{lastWatched}</Text>
            </div>
        </button>
    );
}
