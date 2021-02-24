import {Container, Grow, useTheme} from "@material-ui/core";
import {useEffect, useRef} from "react";
import {startContentAnimation} from "../Focus";

/**
 * @param {{maxWidth: 'xs'|'sm'|'md'|'lg'|'xl'|boolean, classes: Object}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function Content(props) {
    const maxWidth = props.maxWidth !== undefined ? props.maxWidth : "sm";
    const root = useRef(null);
    const theme = useTheme();
    useEffect(() => {
        const duration = theme.transitions.getAutoHeightDuration(root.current.clientHeight);
        startContentAnimation(duration * 0.5);
    });
    return (
        <Grow in>
            <Container maxWidth={maxWidth}
                       classes={props.classes}
                       ref={root}>
                {props.children}
            </Container>
        </Grow>
    );
}