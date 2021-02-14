import {createMuiTheme} from "@material-ui/core";
import {orange} from "@material-ui/core/colors";

const black = {
    default: "#000",
    paper: "#212121"
};

const darkTheme = createMuiTheme({
    palette: {
        type: "dark",
        primary: orange,
        background: black
    },
    transitions: {
        duration: {
            complex: 1000
        }
    }
});

export default darkTheme;