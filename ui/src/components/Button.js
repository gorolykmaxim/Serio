import React from 'react';
import './Button.css';

export default function Button(props) {
    const isSubmit = props.submit || false;
    const isDisabled = props.disabled || false;
    const className = ['serio-button', 'serio-text-font'];
    if (props.className) {
        className.push(props.className);
    }
    if (isDisabled) {
        className.push('serio-button-disabled');
    } else {
        className.push("serio-button-enabled");
    }
    return (
        <button className={className.join(' ')}
                onClick={props.onClick}
                disabled={isDisabled}
                type={isSubmit ? 'submit' : 'button'}
                autoFocus={props.autoFocus}>
            {props.text}
        </button>
    );
}
