import {Box, Paper} from "@material-ui/core";
import ChildAppBar from "../common/ChildAppBar";
import FocusableList from "../common/FocusableList";
import Content from "../common/Content";

function settingToListItem(setting) {
    return {
        primaryText: setting.name,
        secondaryText: setting.value,
        icon: setting.icon,
        selectEvent: setting.selectEvent
    };
}

/**
 * @param {{appBar: Object, sendEvent: Function, items: Array, onSelect: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
function Settings(props) {
    return (
        <Box>
            <ChildAppBar {...props}/>
            <Content>
                <Paper>
                    <FocusableList autoFocus
                                   items={props.items.map(settingToListItem)}
                                   onSelect={i => props.onSelect(i)}/>
                </Paper>
            </Content>
        </Box>
    );
}

export default function create(data, sendEvent) {
    return <Settings appBar={data.appBar}
                     sendEvent={sendEvent}
                     items={data.settings}
                     onSelect={i => sendEvent(i.selectEvent)}/>;
}