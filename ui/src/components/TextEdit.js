import React from 'react';
import './TextEdit.css';

export default function TextEdit(props) {
    return <input className={`serio-text-edit ${props.className || ''}`}
                  type={'text'}
                  required={props.required}
                  name={props.name}
                  defaultValue={props.defaultValue}
                  disabled={props.disabled || false}
                  placeholder={props.placeholder}
                  autoFocus={props.autoFocus}/>;
}
