import React from 'react';
import ReactDOM from 'react-dom';
import Settings from "./settings/Settings";
import {CssBaseline, ThemeProvider} from "@material-ui/core";
import theme from "./Theme";

ReactDOM.render(
    <React.StrictMode>
        <ThemeProvider theme={theme}>
            <CssBaseline/>
            <Settings/>
        </ThemeProvider>
    </React.StrictMode>,
  document.getElementById('root')
);