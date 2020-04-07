import React from "react";
import './Text.css';

export const HEADLINE_1 = 'headline-1';
export const HEADLINE_2 = 'headline-2';
export const HEADLINE_3 = 'headline-3';
export const HEADLINE_4 = 'headline-4';
export const HEADLINE_5 = 'headline-5';
export const HEADLINE_6 = 'headline-6';
export const SUBTITLE_1 = 'subtitle-1';
export const SUBTITLE_2 = 'subtitle-2';
export const BODY_1 = 'body-1';
export const BODY_2 = 'body-2';

export function Text(props) {
    const className = ['serio-text-font', `serio-text-${props.type || BODY_1}`];
    if (props.primary) {
        className.push('serio-text-color-primary');
    }
    return <span className={className.join(' ')}>{props.children}</span>;
}
