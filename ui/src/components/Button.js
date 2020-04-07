import React from 'react';
import './Button.css';

export default function Button(props) {
    const isDisabled = props.disabled || false;
    const className = ['serio-button', 'serio-text-font'];
    if (isDisabled) {
        className.push('serio-button-disabled');
    } else {
        className.push("serio-button-enabled");
    }
    return (
        <button className={className.join(' ')}
                onClick={props.onClick}
                disabled={isDisabled}
                autoFocus={props.autoFocus}>
            {props.text}
        </button>
    );
}
