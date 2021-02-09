import 'typeface-passion-one';
import {Grid, makeStyles, Typography} from "@material-ui/core";
import {maxHeight} from "../Styles";

const logoStyle = makeStyles(() => ({
    root: {
        fontFamily: "'Passion One', cursive",
    }
}));

function Logo() {
    return (
        <Grid container justify="center" alignItems="center" classes={maxHeight()}>
            <Typography variant="h1" className={logoStyle().root} color="primary">Serio</Typography>
        </Grid>
    );
}

export default function create() {
    return <Logo/>;
}