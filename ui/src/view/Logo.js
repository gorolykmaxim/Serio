import 'typeface-passion-one';
import {Fade, makeStyles, useTheme} from "@material-ui/core";
import CenteredView from "../common/CenteredView";
import Text from "../common/Text";

const logoStyle = makeStyles(() => ({
    root: {
        fontFamily: "'Passion One', cursive",
    }
}));

function Logo() {
    return (
        <Fade in timeout={useTheme().transitions.duration.complex}>
            <CenteredView animate={false}>
                <Text variant="h1" className={logoStyle().root} color="primary">Serio</Text>
            </CenteredView>
        </Fade>
    );
}

export default function create() {
    return <Logo/>;
}