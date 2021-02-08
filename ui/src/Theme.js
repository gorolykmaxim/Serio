import {createMuiTheme} from "@material-ui/core";
import {orange} from "@material-ui/core/colors";

const black = {
    default: "#212121",
    paper: "#333"
};

const darkTheme = createMuiTheme({
    palette: {
        type: "dark",
        primary: orange,
        background: black
    }
});

export default darkTheme;