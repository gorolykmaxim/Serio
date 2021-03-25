import {Box, FormControl, Grid, InputBase, makeStyles, Paper} from "@material-ui/core";
import {Search as SearchIcon} from "@material-ui/icons";
import ChildAppBar from "../common/ChildAppBar";
import {callOnEnter} from "../common/BrowserEvents";
import Content from "../common/Content";
import TvShowGrid from "../common/TvShowGrid";

const searchBarStyles = makeStyles(theme => ({
    root: {
        marginLeft: theme.spacing(1),
        verticalAlign: "middle"
    },
}));

/**
 * @param {{placeholder: string, value: string, autoFocus: boolean, onValueChange: Function, onSearch: Function, onBack: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
function SearchBar(props) {
    return (
        <ChildAppBar onBack={props.onBack}>
            <Grid container justify="center">
                <Paper>
                    <SearchIcon classes={searchBarStyles()}/>
                    <FormControl classes={searchBarStyles()}>
                        <InputBase autoFocus={props.autoFocus}
                                   placeholder={props.placeholder}
                                   onKeyDown={callOnEnter(props.onSearch)}
                                   onChange={e => props.onValueChange(e.target.value)}
                                   value={props.value}/>
                    </FormControl>
                </Paper>
            </Grid>
        </ChildAppBar>
    );
}

/**
 * @param {{
 * searchText: string,
 * searchString: string,
 * tvShows: Array,
 * onTvShowSelect: Function,
 * onSearchStringChange: Function,
 * onSearch: Function,
 * onBack: Function,
 * selected: number
 * }} props
 * @returns {JSX.Element}
 * @constructor
 */
function Search(props) {
    return (
        <Box>
            <SearchBar placeholder={props.searchText}
                       value={props.searchString}
                       autoFocus={props.selected === undefined}
                       onValueChange={props.onSearchStringChange}
                       onSearch={props.onSearch}
                       onBack={props.onBack}/>
            <Content maxWidth={false}>
                <TvShowGrid selected={props.selected}
                            tvShows={props.tvShows}
                            onSelect={props.onTvShowSelect}/>
            </Content>
        </Box>
    );
}

export default function create(data, sendEvent) {
    return <Search searchText={data.searchText}
                   searchString={data.searchString}
                   selected={data.selected}
                   tvShows={data.tvShows}
                   onTvShowSelect={sendEvent}
                   onSearchStringChange={v => sendEvent(Object.assign({searchString: v}, data.searchStringChangeEvent))}
                   onSearch={() => sendEvent(data.searchEvent)}
                   onBack={() => sendEvent(data.backEvent)}/>;
}