import {Box, FormControl, Grid, InputBase, makeStyles, Paper} from "@material-ui/core";
import {Search as SearchIcon} from "@material-ui/icons";
import ChildAppBar from "../common/ChildAppBar";
import {callOnEnter} from "../common/BrowserEvents";
import Content from "../common/Content";
import CardGrid from "../common/CardGrid";

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
        <ChildAppBar appBar={{}} sendEvent={props.onBack}>
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
 * emptyGridPlaceholderText: string,
 * items: Array,
 * onSelect: Function,
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
                       autoFocus={props.items.length === 0 || props.selected === undefined}
                       onValueChange={props.onSearchStringChange}
                       onSearch={props.onSearch}
                       onBack={props.onBack}/>
            <Content maxWidth={false}>
                <CardGrid selected={props.selected}
                          items={props.items}
                          emptyGridPlaceholderText={props.emptyGridPlaceholderText}
                          onSelect={props.onSelect}/>
            </Content>
        </Box>
    );
}

export default function create(data, sendEvent) {
    return <Search searchText={data.searchText}
                   searchString={data.searchString}
                   selected={data.selected}
                   items={data.items}
                   emptyGridPlaceholderText={data.emptyGridPlaceholderText}
                   onSelect={sendEvent}
                   onSearchStringChange={v => sendEvent(Object.assign({searchString: v}, data.searchStringChangeEvent))}
                   onSearch={() => sendEvent(data.searchEvent)}
                   onBack={() => sendEvent(data.backEvent)}/>;
}