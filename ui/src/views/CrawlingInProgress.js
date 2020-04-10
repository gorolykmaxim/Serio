import React from "react";
import './CrawlingInProgress.css';
import CircularProgress from "../components/CircularProgress";
import {HEADLINE_5, Text} from "../components/Text";

export default function CrawlingInProgress(props) {
    return (
        <div className='serio-crawling-in-progress serio-full-height'>
            <CircularProgress/>
            <Text type={HEADLINE_5} primary className='serio-crawling-in-progress-margin'>Crawling in progress. Please standby.</Text>
        </div>
    );
}
