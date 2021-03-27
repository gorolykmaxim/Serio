import Dialog from "./Dialog";
import EditText from "../common/EditText";

/**
 * @param {{dialog: Object, editText: Object, sendEvent: Function}} props
 * @returns {JSX.Element}
 */
export default function EditTextDialog(props) {
    return <Dialog autoFocus={false} {...props}><EditText {...props}/></Dialog>;
}