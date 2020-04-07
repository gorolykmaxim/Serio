import React from 'react';
import './TextArea.css';

export default function TextArea(props) {
    return (
        <textarea className={"serio-text-area " + (props.className || '')}
                  name={props.name}
                  placeholder={props.placeholder}
                  autoFocus={props.autoFocus}>
            {props.defaultValue}
        </textarea>
    );
}
