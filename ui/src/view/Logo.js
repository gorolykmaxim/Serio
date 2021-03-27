import 'typeface-passion-one';
import {Fade, makeStyles, useTheme} from "@material-ui/core";
import CenteredView from "../common/CenteredView";
import Text from "../common/Text";

const logoStyle = makeStyles(() => ({
    root: {
        fontFamily: "'Passion One', cursive",
    }
}));

export default function Logo() {
    return (
        <Fade in timeout={useTheme().transitions.duration.complex}>
            <CenteredView>
                <Text variant="h1" className={logoStyle().root} color="primary">Serio</Text>
            </CenteredView>
        </Fade>
    );
}