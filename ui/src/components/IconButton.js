import React from "react";
import './IconButton.css';
import AutoFocusable from "./AutoFocusable";

export const MEDIUM_SIZE = 0;
export const LARGE_SIZE = 1;

export class IconButton extends AutoFocusable {
    render() {
        const iconClassName = ['material-icons'];
        if (this.props.size === LARGE_SIZE) {
            iconClassName.push('serio-icon-button-large-size');
        } else if (this.props.size === MEDIUM_SIZE) {
            iconClassName.push('serio-icon-button-medium-size');
        } else {
            iconClassName.push('serio-icon-button-normal-size');
        }
        const buttonClassName = ['serio-icon-button'];
        if (this.props.isDisabled) {
            buttonClassName.push('serio-icon-button-disabled');
        } else {
            buttonClassName.push('serio-icon-button-enabled')
        }
        if (this.props.className) {
            buttonClassName.push(this.props.className);
        }
        return (
            <button disabled={this.props.isDisabled}
                    className={buttonClassName.join(' ')}
                    onClick={e => {
                        if (this.props.onClickStopPropagate) {
                            e.stopPropagation();
                            this.props.onClickStopPropagate();
                        } else if (this.props.onClick) {
                            this.props.onClick();
                        }
                    }}
                    ref={root => {this.focusable = root}}>
                <span className={iconClassName.join(' ')}>{this.props.icon}</span>
            </button>
        );
    }
}
