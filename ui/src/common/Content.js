import {Container, Grow} from "@material-ui/core";

/**
 * @param {{maxWidth: 'xs'|'sm'|'md'|'lg'|'xl'|boolean, classes: Object}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function Content(props) {
    const maxWidth = props.maxWidth !== undefined ? props.maxWidth : "sm";
    return (
        <Grow in>
            <Container maxWidth={maxWidth} classes={props.classes}>
                {props.children}
            </Container>
        </Grow>
    );
}