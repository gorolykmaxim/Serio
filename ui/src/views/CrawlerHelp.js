import React from "react";
import './CrawlerHelp.css';
import {BODY_1, HEADLINE_5, Text} from "../components/Text";
import Button from "../components/Button";
import {getFunction} from "../common";

const crawlerExample = JSON.stringify({
    "steps": [
        {
            "type": "value",
            "value": "http://tv-show.com/my-show"
        },
        {
            "type": "fetch"
        },
        {
            "type": "regExp",
            "regExp": "<video src=(.*)/>"
        }
    ]
}, null, ' ');

export default function CrawlerHelp(props) {
    const onBack = getFunction(props.onBack);
    return <div className='serio-crawler-help serio-full-height'>
        <div className='serio-padding'>
            <Text type={HEADLINE_5} primary>Help</Text>
        </div>
        <div className='serio-padding'>
            <Button text='back' onClick={onBack} autoFocus={true}/>
        </div>
        <div className='serio-vertical-container serio-crawler-help-scrollable-container'>
            <div className='serio-padding'>
                <Text type={BODY_1} wrap={true}>Crawler is a thing, that goes to an external website and crawls
                    it's content searching for various data (episode videos crawler searches for links to video files
                    of TV show episodes).</Text>
            </div>
            <div className='serio-padding'>
                <Text type={BODY_1} wrap={true}>A crawler consists of an array of steps. When a crawler is executed
                    all of it's steps are executed in their specified order. Each step produces an output data,
                    which in turn is getting assigned to the following step as an input data to operate on.
                    Thus, you can treat a crawler as a data transformation pipeline where in the end there should be
                    a link (for thumbnail crawler) or a list of links (for episode video crawler).</Text>
            </div>
            <div className='serio-padding'>
                <Text type={BODY_1} wrap={true}>The data, that is being passed between the crawler steps,
                    is just an array of strings: each step accepts a string array as its input and produces
                    a string array as its output. First step in each crawler is kind of special because it receives
                    an empty array as an input (episode names crawler is an exception: there the first step
                    receives the list of links to episode video files, crawled by the episode videos crawler).</Text>
            </div>
            <div className='serio-padding'>
                <Text type={BODY_1} wrap={true}>Thumbnail crawler will use the first string from the result array as
                    a TV show thumbnail image URL. Episode video crawler will treat strings from the result array as
                    episode video links in the order in which they were crawled, meaning the first link will be
                    considered the first episode, the second link - the second episode etc. Episode name crawler
                    will work the same way the episode video crawler does: every string will be used as an episode name
                    of a corresponding episode.</Text>
            </div>
            <div className='serio-padding'>
                <Text type={BODY_1} wrap={true}>Here is an example of a crawler:</Text>
            </div>
            <div className='serio-padding'>
                <pre>{crawlerExample}</pre>
            </div>
            <div className='serio-padding'>
                <Text type={BODY_1} wrap={true}>Step with "type": "value" ignores the input data and returns an array
                    with a single string, specified in "value": "string-to-return".</Text>
            </div>
            <div className='serio-padding'>
                <Text type={BODY_1} wrap={true}>Step with "type": "fetch" treats every string from the input array as
                    a link, fetches content of such link and returns an array of fetched contents.</Text>
            </div>
            <div className='serio-padding'>
                <Text type={BODY_1} wrap={true}>Step with "type": "regExp" applies the regular expression, specified in
                    "regExp": "regular-expression-to-apply", to every string from the input array, and returns an array
                    of all found strings across all the input strings. Regular expressions can return groups of results,
                    if you specify "(.*)" as a part of an expression. Normally this step returns all matches from
                    the group 0 but if the aforementioned construction is present in the expression - the group 0 will
                    be ignored and only the other groups (that correspond to such expressions) will be returned.</Text>
            </div>
            <div className='serio-padding'>
                <Text type={BODY_1} wrap={true}>Step with "type": "transform" will transform each string
                    from the input array using the "template": "transformation-template-to-apply" and return
                    the results. Let's say that you have a following array of input strings: ['1', '2', '3'].
                    A transform step with "template": "Episode %s" will turn that array into
                    ['Episode 1', 'Episode 2', 'Episode 3']. See Java's String.format() for more details.</Text>
            </div>
        </div>
    </div>;
}