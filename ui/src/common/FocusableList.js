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
                  onClick={props.onClick}
                  autoFocus={props.autoFocus}
                  onFocus={() => setSelected(true)}
                  onBlur={() => setSelected(false)}>
            {icon}
            <ListItemText primary={props.primaryText} secondary={props.secondaryText}/>
        </ListItem>
    );
}

/**
 * @param {{autoFocus: boolean, selected: number, onSelect: Function, items: Array}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function FocusableList(props) {
    const items = [];
    for (let i = 0; i < props.items.length; i++) {
        const item = props.items[i];
        items.push((
            <FocusableListItem key={item.primaryText}
                               autoFocus={props.autoFocus && (props.selected ? props.selected === i : i === 0)}
                               onClick={() => props.onSelect(item)}
                               primaryText={item.primaryText}
                               secondaryText={item.secondaryText}
                               icon={item.icon}/>
        ));
    }
    return (
        <List>
            {items}
        </List>
    );
}