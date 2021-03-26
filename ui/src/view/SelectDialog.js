import {Box} from "@material-ui/core";
import ChildAppBar from "../common/ChildAppBar";
import FocusableList from "../common/FocusableList";
import Content from "../common/Content";

/**
 * @param {{appBar: Object, sendEvent: Function, selectedValue: string, values: Array<string>, onSelect: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
function SelectDialog(props) {
    return (
        <Box>
            <ChildAppBar {...props}/>
            <Content>
                <FocusableList autoFocus
                               selected={props.selectedValue ? props.values.indexOf(props.selectedValue) : 0}
                               items={props.values.map(v => ({primaryText: v}))}
                               onSelect={i => props.onSelect(i.primaryText)}/>
            </Content>
        </Box>
    );
}

export default function create(data, sendEvent) {
    return <SelectDialog appBar={data.appBar}
                         sendEvent={sendEvent}
                         selectedValue={data.selectedValue}
                         values={data.values}
                         onSelect={(option) => sendEvent(Object.assign({value: option}, data.selectEvent))}/>;
}