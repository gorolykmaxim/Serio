import React from "react";
import './IconButton.css';

export default function IconButton(props) {
    return (
        <button className='serio-icon-button' onClick={props.onClick} autoFocus={props.autoFocus} >
            <span className={`material-icons ${props.large ? 'serio-icon-button-large-size' : 'serio-icon-button-normal-size'}`}>{props.icon}</span>
        </button>
    );
}
