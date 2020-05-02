import React from "react";
import './ListTile.css';
import {BODY_1, BODY_2, Text} from "./Text";
import {BUG_REPORT} from "./Icons";

export default function ListTile(props) {
    let icon;
    if (props.icon) {
        icon = <span className="material-icons">{props.icon}</span>;
    } else if (props.icon !== null) {
        icon = <span className="material-icons serio-list-tile-icon-placeholder">{BUG_REPORT}</span>;
    }
    const className = ['serio-list-tile', 'serio-padding'];
    if (props.hoverable) {
        className.push('serio-list-tile-hoverable');
    }
    if (props.className) {
        className.push(props.className);
    }
    return (
        <div className={className.join(' ')} tabIndex={0} onClick={props.onClick} onFocusCapture={props.onFocusCapture}>
            {icon}
            <div className='serio-list-tile-content serio-margin-before'>
                <Text type={BODY_1}>{props.primaryText}</Text>
                <Text type={BODY_2}>{props.secondaryText}</Text>
            </div>
        </div>
    );
}
