import {Box, FormControl, Grid, IconButton, InputBase, makeStyles, Paper} from "@material-ui/core";
import {Search as SearchIcon} from "@material-ui/icons";
import ChildAppBar from "../common/ChildAppBar";
import {callOnEnter} from "../common/BrowserEvents";

const searchBarStyles = makeStyles(theme => ({
    root: {
        marginLeft: theme.spacing(2),
        verticalAlign: "middle"
    },
}));

/**
 * @param {{placeholder: string, value: string, onValueChange: Function, onSearch: Function, onBack: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
function SearchBar(props) {
    return (
        <ChildAppBar color="transparent" elevation={0} onBack={props.onBack}>
            <Grid container justify="center">
                <Paper>
                    <FormControl classes={searchBarStyles()}>
                        <InputBase autoFocus
                                   placeholder={props.placeholder}
                                   onKeyDown={callOnEnter(props.onSearch)}
                                   onChange={e => props.onValueChange(e.target.value)}
                                   value={props.value}/>
                    </FormControl>
                    <IconButton onClick={props.onSearch}>
                        <SearchIcon/>
                    </IconButton>
                </Paper>
            </Grid>
        </ChildAppBar>
    );
}

/**
 * @param {{searchText: string, searchString: string, onSearchStringChange: Function, onSearch: Function, onBack: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
function Search(props) {
    return (
        <Box>
            <SearchBar placeholder={props.searchText}
                       value={props.searchString}
                       onValueChange={props.onSearchStringChange}
                       onSearch={props.onSearch}
                       onBack={props.onBack}/>
        </Box>
    );
}

export default function create(data, sendEvent) {
    return <Search searchText={data.searchText}
                   searchString={data.searchString}
                   onSearchStringChange={v => sendEvent(Object.assign({searchString: v}, data.searchStringChangeEvent))}
                   onSearch={() => sendEvent(data.searchEvent)}
                   onBack={() => sendEvent(data.backEvent)}/>;
}