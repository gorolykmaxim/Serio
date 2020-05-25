import React from 'react';
import './Button.css';
import AutoFocusable from "./AutoFocusable";

export default class Button extends AutoFocusable {
    render() {
        const isSubmit = this.props.submit || false;
        const isDisabled = this.props.disabled || false;
        const className = ['serio-button', 'serio-text-font'];
        if (this.props.className) {
            className.push(this.props.className);
        }
        if (isDisabled) {
            className.push('serio-button-disabled');
        } else {
            className.push("serio-button-enabled");
        }
        if (this.props.hoverable !== false) {
            className.push('serio-button-hoverable');
        }
        return (
            <button className={className.join(' ')}
                    ref={root => {this.focusable = root}}
                    onClick={this.props.onClick}
                    disabled={isDisabled}
                    type={isSubmit ? 'submit' : 'button'}>
                {this.props.text}
            </button>
        );
    }
}
