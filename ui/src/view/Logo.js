import 'typeface-passion-one';
import {Fade, Grid, makeStyles, Typography, useTheme} from "@material-ui/core";
import {maxHeight} from "../Styles";

const logoStyle = makeStyles(() => ({
    root: {
        fontFamily: "'Passion One', cursive",
    }
}));

function Logo() {
    return (
        <Fade in timeout={useTheme().transitions.duration.complex}>
            <Grid container justify="center" alignItems="center" classes={maxHeight()}>
                <Typography variant="h1" className={logoStyle().root} color="primary">Serio</Typography>
            </Grid>
        </Fade>
    );
}

export default function create() {
    return <Logo/>;
}