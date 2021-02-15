import 'typeface-passion-one';
import {Fade, Grid, makeStyles, useTheme} from "@material-ui/core";
import {maxHeight} from "../Styles";
import Text from "../common/Text";

const logoStyle = makeStyles(() => ({
    root: {
        fontFamily: "'Passion One', cursive",
    }
}));

function Logo() {
    return (
        <Fade in timeout={useTheme().transitions.duration.complex}>
            <Grid container justify="center" alignItems="center" classes={maxHeight()}>
                <Text variant="h1" className={logoStyle().root} color="primary">Serio</Text>
            </Grid>
        </Fade>
    );
}

export default function create() {
    return <Logo/>;
}