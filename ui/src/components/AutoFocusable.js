import React from "react";
import {getFunction} from "../common";

export default class AutoFocusable extends React.Component {
    constructor(props, context) {
        super(props, context);
        this._focusable = null;
        this._onFocusableSet = getFunction(this.props.focusableRef);
    }

    set focusable(value) {
        this._focusable = value;
        this._onFocusableSet(this._focusable);
    }

    componentDidMount() {
        if (this.props.autoFocus) {
            this._focusable.focus();
        }
    }
}
