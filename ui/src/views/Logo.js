import React from "react";
import './Logo.css';
import {HEADLINE_1, Text} from "../components/Text";

export default function Logo(props) {
    return (
        <div className='serio-logo serio-full-height'>
            <Text type={HEADLINE_1} primary className='serio-logo-text'>Serio</Text>
        </div>
    );
}
