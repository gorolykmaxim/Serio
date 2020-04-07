import React from "react";
import './ListTile.css';
import {BODY_1, BODY_2, Text} from "./Text";

export default function ListTile(props) {
    let icon;
    if (props.icon) {
        icon = <span className="material-icons">{props.icon}</span>;
    } else {
        icon = <span className="serio-list-tile-icon-placeholder"/>;
    }
    return (
        <div className='serio-list-tile serio-padding' tabIndex={0} onClick={props.onClick}>
            {icon}
            <div className='serio-list-tile-content serio-margin-before'>
                <Text type={BODY_1}>{props.primaryText}</Text>
                <Text type={BODY_2}>{props.secondaryText}</Text>
            </div>
        </div>
    );
}
