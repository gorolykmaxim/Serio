import {List, ListItem, ListItemIcon, ListItemText} from "@material-ui/core";
import {createElement, useState} from "react";
import * as Icons from "@material-ui/icons";

/**
 * @param {{autoFocus: boolean, onClick: Function, primaryText: string, secondaryText: string, icon: string}} props
 * @returns {JSX.Element}
 * @constructor
 */
function FocusableListItem(props) {
    const [selected, setSelected] = useState(false);
    const icon = props.icon ? <ListItemIcon>{createElement(Icons[props.icon])}</ListItemIcon> : null;
    return (
        <ListItem button
                  selected={selected}
                  autoFocus={props.autoFocus}
                  onClick={props.onClick}
                  onFocus={() => setSelected(true)}
                  onBlur={() => setSelected(false)}>
            {icon}
            <ListItemText primary={props.primaryText} secondary={props.secondaryText}/>
        </ListItem>
    );
}

/**
 * @param {{autoFocus: boolean, list: Object, sendEvent: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function FocusableList(props) {
    const {selected, items} = props.list;
    const listItems = [];
    for (let i = 0; i < items.length; i++) {
        const item = items[i];
        listItems.push((
            <FocusableListItem key={item.primaryText}
                               autoFocus={props.autoFocus && (selected ? selected === i : i === 0)}
                               onClick={() => props.sendEvent(item.selectEvent)}
                               primaryText={item.primaryText}
                               secondaryText={item.secondaryText}
                               icon={item.icon}/>
        ));
    }
    return (
        <List>
            {listItems}
        </List>
    );
}