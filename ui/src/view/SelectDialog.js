import {Box, Container, List, ListItemText} from "@material-ui/core";
import ChildAppBar from "../common/ChildAppBar";
import FocusableListItem from "../common/FocusableListItem";

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
            <Container maxWidth="sm">
                <List>
                    {props.values.map(value => (
                        <FocusableListItem key={value} autoFocus={value === props.selectedValue} onClick={() => props.onSelect(value)}>
                            <ListItemText primary={value}/>
                        </FocusableListItem>
                    ))}
                </List>
            </Container>
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