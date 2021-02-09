import {useState} from "react";
import {ListItem} from "@material-ui/core";

/**
 * @param {{children: Array<JSX.Element>, autoFocus: boolean, onClick: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function FocusableListItem(props) {
    const [selected, setSelected] = useState(false);
    return (
        <ListItem button
                  selected={selected}
                  onClick={props.onClick}
                  autoFocus={props.autoFocus}
                  onFocus={() => setSelected(true)}
                  onBlur={() => setSelected(false)}>
            {props.children}
        </ListItem>
    );
}