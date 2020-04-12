import React from "react";
import './ShowCard.css';
import {HEADLINE_5, SUBTITLE_1, Text} from "./Text";
import {getFunction} from "../common";
import AutoFocusable from "./AutoFocusable";

export default class ShowCard extends AutoFocusable {
    render() {
        const {name, thumbnail, lastWatched} = this.props.show;
        const backgroundStyle = {backgroundImage: `url(${thumbnail})`};
        const onClick = getFunction(this.props.onClick);
        const className = ['serio-show-card'];
        if (this.props.hoverable) {
            className.push('serio-show-card-hoverable');
        }
        if (this.props.className) {
            className.push(this.props.className);
        }
        return (
            <button className={className.join(' ')}
                    style={backgroundStyle}
                    onFocusCapture={this.props.onFocusCapture}
                    ref={root => {this.focusable = root}}
                    onClick={() => onClick(this.props.show)}>
                <div className='serio-show-card-inside-container'>
                    <Text type={HEADLINE_5}>{name}</Text>
                    <Text type={SUBTITLE_1}>{lastWatched}</Text>
                </div>
            </button>
        );
    }
}
