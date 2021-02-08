import 'typeface-passion-one';
import {Grid, makeStyles, Typography} from "@material-ui/core";
import {maxHeight} from "../common/Styles";

const logoStyle = makeStyles(() => ({
    root: {
        fontFamily: "'Passion One', cursive",
    }
}));

export default function Logo() {
    return (
        <Grid container justify="center" alignItems="center" classes={maxHeight()}>
            <Typography variant="h1" className={logoStyle().root} color="primary">Serio</Typography>
        </Grid>
    );
}