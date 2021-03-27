import {Box} from "@material-ui/core";
import ChildAppBar from "../common/ChildAppBar";
import FocusableList from "../common/FocusableList";
import Content from "../common/Content";

/**
 * @param {{appBar: Object, list: Object, sendEvent: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function SelectDialog(props) {
    return (
        <Box>
            <ChildAppBar {...props}/>
            <Content>
                <FocusableList autoFocus {...props}/>
            </Content>
        </Box>
    );
}