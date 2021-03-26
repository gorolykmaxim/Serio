import {Box, Paper} from "@material-ui/core";
import ChildAppBar from "../common/ChildAppBar";
import FocusableList from "../common/FocusableList";
import Content from "../common/Content";

/**
 * @param {{appBar: Object, list: Object, sendEvent: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
function Settings(props) {
    return (
        <Box>
            <ChildAppBar {...props}/>
            <Content>
                <Paper>
                    <FocusableList autoFocus {...props}/>
                </Paper>
            </Content>
        </Box>
    );
}

export default function create(data, sendEvent) {
    return <Settings sendEvent={sendEvent} {...data}/>;
}