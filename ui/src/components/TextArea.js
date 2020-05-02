import React from 'react';
import './TextArea.css';
import AutoFocusable from "./AutoFocusable";
import {getFunction} from "../common";

export default class TextArea extends AutoFocusable {
    render() {
        const onChange = getFunction(this.props.onChange);
        return (
            <textarea className={`serio-text-area ${this.props.className || ''}`}
                      required={this.props.required}
                      name={this.props.name}
                      onChange={e => onChange(e.target.value)}
                      defaultValue={this.props.defaultValue}
                      placeholder={this.props.placeholder}
                      ref={root => {this.focusable = root}}/>
        );
    }
}
