import React from "react";

export default class AutoFocusable extends React.Component {
    componentDidMount() {
        if (this.props.autoFocus) {
            this.focusable.focus();
        }
    }
}
