import {ListItemIcon, ListItemText} from "@material-ui/core";
import FocusableListItem from "./FocusableListItem";

/**
 * @param {{name: string, value: string, icon: JSX.Element, autoFocus: boolean, onClick: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function SettingItem(props) {
    return (
        <FocusableListItem autoFocus={props.autoFocus} onClick={props.onClick}>
            <ListItemIcon>
                {props.icon}
            </ListItemIcon>
            <ListItemText primary={props.name}
                          secondary={props.value}/>
        </FocusableListItem>
    );
}