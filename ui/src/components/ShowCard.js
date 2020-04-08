import React from "react";
import './ShowCard.css';
import {HEADLINE_5, SUBTITLE_1, Text} from "./Text";

export default function ShowCard(props) {
    const {name, thumbnail, lastWatched} = props.show;
    const backgroundStyle = {backgroundImage: `url(${thumbnail})`};
    const onClick = props.onClick || (() => {});
    return (
        <button className={`serio-show-card ${props.className || ''}`}
                style={backgroundStyle}
                autoFocus={props.autoFocus}
                onClick={() => onClick(props.show)}>
            <div className='serio-show-card-inside-container'>
                <Text type={HEADLINE_5}>{name}</Text>
                <Text type={SUBTITLE_1}>{lastWatched}</Text>
            </div>
        </button>
    );
}
