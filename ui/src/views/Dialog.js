import React from "react";
import './Dialog.css';
import {BODY_1, HEADLINE_5, Text} from "../components/Text";
import Button from "../components/Button";

export default function Dialog(props) {
    console.assert(props.title);
    console.assert(props.text);
    console.assert(props.actions && props.actions.length > 0);
    const actions = props.actions.map(a =>
        <Button text={a.name}
                onClick={a.callback}
                autoFocus={a.isDefault}
                className='serio-margin-before'/>
    );
    return (
        <div className='serio-dialog serio-full-height'>
            <div className='serio-dialog-content'>
                <Text type={HEADLINE_5} primary className='serio-margin' wrap>{props.title}</Text>
                <Text type={BODY_1} className='serio-margin' wrap>{props.text}</Text>
                <div className='serio-form-actions serio-margin'>
                    {actions}
                </div>
            </div>
        </div>
    );
}
