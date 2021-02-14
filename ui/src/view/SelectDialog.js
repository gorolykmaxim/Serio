import {Box} from "@material-ui/core";
import ChildAppBar from "../common/ChildAppBar";
import FocusableList from "../common/FocusableList";
import Content from "../common/Content";

/**
 * @param {{title: string, selectedValue: string, values: Array<string>, onSelect: Function, onCancel: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
function SelectDialog(props) {
    return (
        <Box>
            <ChildAppBar title={props.title}
                         color="transparent"
                         elevation={0}
                         onBack={props.onCancel}/>
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
    return <SelectDialog title={data.title}
                         selectedValue={data.selectedValue}
                         values={data.values}
                         onSelect={(option) => sendEvent(Object.assign({value: option}, data.selectEvent))}
                         onCancel={() => sendEvent(data.cancelEvent)}/>;
}