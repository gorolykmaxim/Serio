import React from 'react';
import './TextEdit.css';
import AutoFocusable from "./AutoFocusable";
import {getFunction} from "../common";

export default class TextEdit extends AutoFocusable {
    render() {
        const onChange = getFunction(this.props.onChange);
        return <input className={`serio-text-edit ${this.props.className || ''}`}
                      type={'text'}
                      required={this.props.required}
                      name={this.props.name}
                      onChange={e => onChange(e.target.value)}
                      defaultValue={this.props.defaultValue}
                      disabled={this.props.disabled || false}
                      placeholder={this.props.placeholder}
                      ref={root => {this.focusable = root}}/>;
    }
}
