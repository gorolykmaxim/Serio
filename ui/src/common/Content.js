import {Container, Grow} from "@material-ui/core";

export default function Content(props) {
    return (
        <Grow in>
            <Container maxWidth="sm">
                {props.children}
            </Container>
        </Grow>
    );
}