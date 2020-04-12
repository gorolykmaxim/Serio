import React from 'react';
import './TextArea.css';

export default function TextArea(props) {
    return (
        <textarea className={`serio-text-area ${props.className || ''}`}
                  required={props.required}
                  name={props.name}
                  defaultValue={props.defaultValue}
                  placeholder={props.placeholder}
                  autoFocus={props.autoFocus}/>
    );
}
