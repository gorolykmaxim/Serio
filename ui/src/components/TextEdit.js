import React from 'react';
import './TextEdit.css';
import AutoFocusable from "./AutoFocusable";

export default class TextEdit extends AutoFocusable {
    render() {
        return <input className={`serio-text-edit ${this.props.className || ''}`}
                      type={'text'}
                      required={this.props.required}
                      name={this.props.name}
                      onChange={this.props.onChange}
                      defaultValue={this.props.defaultValue}
                      disabled={this.props.disabled || false}
                      placeholder={this.props.placeholder}
                      ref={root => {this.focusable = root}}/>;
    }
}
