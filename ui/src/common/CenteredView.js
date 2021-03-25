import {maxHeight} from "../Styles";
import {Container, Grid, Grow} from "@material-ui/core";

/**
 * @param {{children: Array<JSX.Element>, animate: boolean}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function CenteredView(props) {
    const height = maxHeight();
    const root = (
        <Container classes={height} maxWidth="xs" {...props}>
            <Grid container
                  classes={height}
                  direction="column"
                  justify="center"
                  alignItems="center">
                {props.children}
            </Grid>
        </Container>
    );
    if (props.animate === false) {
        return root;
    } else {
        return <Grow in>{root}</Grow>;
    }
}