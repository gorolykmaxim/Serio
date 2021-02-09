import {maxHeight} from "../common/Styles";
import {Container, Grid} from "@material-ui/core";

/**
 * @param {{children: Array<JSX.Element>}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function CenteredView(props) {
    const height = maxHeight();
    return (
        <Container classes={height} maxWidth="xs">
            <Grid container
                  classes={height}
                  direction="column"
                  justify="center"
                  alignItems="center">
                {props.children}
            </Grid>
        </Container>
    );
}