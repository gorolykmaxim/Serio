import React from 'react';
import './TextArea.css';
import AutoFocusable from "./AutoFocusable";

export default class TextArea extends AutoFocusable {
    render() {
        return (
            <textarea className={`serio-text-area ${this.props.className || ''}`}
                      required={this.props.required}
                      name={this.props.name}
                      onChange={this.props.onChange}
                      defaultValue={this.props.defaultValue}
                      placeholder={this.props.placeholder}
                      ref={root => {this.focusable = root}}/>
        );
    }
}
