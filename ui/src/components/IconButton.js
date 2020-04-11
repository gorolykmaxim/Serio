import React from "react";
import './IconButton.css';

export const MEDIUM_SIZE = 0;
export const LARGE_SIZE = 1;

export function IconButton(props) {
    const iconClassName = ['material-icons'];
    if (props.size === LARGE_SIZE) {
        iconClassName.push('serio-icon-button-large-size');
    } else if (props.size === MEDIUM_SIZE) {
        iconClassName.push('serio-icon-button-medium-size');
    } else {
        iconClassName.push('serio-icon-button-normal-size');
    }
    const buttonClassName = ['serio-icon-button'];
    if (props.isDisabled) {
        buttonClassName.push('serio-icon-button-disabled');
    } else {
        buttonClassName.push('serio-icon-button-enabled')
    }
    if (props.className) {
        buttonClassName.push(props.className);
    }
    return (
        <button disabled={props.isDisabled}
                className={buttonClassName.join(' ')}
                onClick={props.onClick}
                autoFocus={props.autoFocus}>
            <span className={iconClassName.join(' ')}>{props.icon}</span>
        </button>
    );
}
