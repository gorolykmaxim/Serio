import {Box, FormControl, Grid, InputBase, makeStyles, Paper} from "@material-ui/core";
import {Search as SearchIcon} from "@material-ui/icons";
import ChildAppBar from "../common/ChildAppBar";
import {callOnEnter} from "../common/BrowserEvents";
import Content from "../common/Content";
import CardGrid from "../common/CardGrid";
import {useValue} from "../ValueState";

const searchBarStyles = makeStyles(theme => ({
    root: {
        marginLeft: theme.spacing(1),
        verticalAlign: "middle"
    },
}));

/**
 * @param {{searchBar: Object, sendEvent: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
function SearchBar(props) {
    const {placeholder, initialValue, valueChangeEvent, searchEvent, backEvent} = props.searchBar;
    const styles = searchBarStyles();
    const [value, onValueChanged] = useValue(initialValue, valueChangeEvent, props.sendEvent);
    return (
        <ChildAppBar appBar={{backEvent}} sendEvent={props.sendEvent}>
            <Grid container justify="center">
                <Paper>
                    <SearchIcon classes={styles}/>
                    <FormControl classes={styles}>
                        <InputBase autoFocus={props.autoFocus}
                                   placeholder={placeholder}
                                   onKeyDown={callOnEnter(() => props.sendEvent(searchEvent))}
                                   onChange={onValueChanged}
                                   value={value}/>
                    </FormControl>
                </Paper>
            </Grid>
        </ChildAppBar>
    );
}

/**
 * @param {{
 * searchBar: Object,
 * sendEvent: Function,
 * items: Array,
 * onSelect: Function,
 * selected: number
 * }} props
 * @returns {JSX.Element}
 * @constructor
 */
function Search(props) {
    return (
        <Box>
            <SearchBar autoFocus={props.items.length === 0 || props.selected === undefined} {...props}/>
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
    return <Search searchBar={data.searchBar}
                   sendEvent={sendEvent}
                   selected={data.selected}
                   items={data.items}
                   emptyGridPlaceholderText={data.emptyGridPlaceholderText}
                   onSelect={sendEvent}/>;
}