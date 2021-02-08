import React from 'react';
import ReactDOM from 'react-dom';
import "./index.css";
import {CssBaseline, ThemeProvider} from "@material-ui/core";
import theme from "./Theme";
import TextFieldDialog from "./settings/TextFieldDialog";

ReactDOM.render(
    <React.StrictMode>
        <ThemeProvider theme={theme}>
            <CssBaseline/>
            {/*<Settings/>*/}
            <TextFieldDialog
                title="Crawler config URL"
                description="Specify URL to the file, that contains configuration of all the crawlers, responsible for crawling tv shows."
                label="Crawler config URL"
                onCancel={() => console.log("Cancel")}
                onSave={() => console.log("Save")}
                onValueChange={(v) => console.log(v)}
                cancelText="Cancel"
                saveText="Save"/>
        </ThemeProvider>
    </React.StrictMode>,
  document.getElementById('root')
);