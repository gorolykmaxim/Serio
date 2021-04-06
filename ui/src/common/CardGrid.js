import {Grid, makeStyles, Paper, useMediaQuery} from "@material-ui/core";
import Text from "./Text";
import TouchRipple from "@material-ui/core/ButtonBase/TouchRipple";
import {useRef} from "react";
import {useAutoFocusWhenReady} from "../Focus";
import {grey} from "@material-ui/core/colors";
import {useEvent} from "./BrowserEvents";
import {debounce} from "throttle-debounce";

const CARD_WIDTH_XS = 6;
const CARD_WIDTH_SM = 4;
const CARD_WIDTH_MD = 2;
const CARD_HEIGHT = 200;
const LOAD_MORE_DEBOUNCE_INTERVAL = 1000;

const emptyGridPlaceholderStyles = makeStyles(() => ({
    root: {
        height: "300px"
    }
}));

/**
 * @param {{placeholderText: string}} props
 * @returns {JSX.Element}
 * @constructor
 */
function EmptyGridPlaceholder(props) {
    return (
        <Grid container classes={emptyGridPlaceholderStyles()} justify="center" alignItems="center">
            <Text variant="h6">{props.placeholderText}</Text>
        </Grid>
    );
}


const cardStyles = makeStyles(theme => ({
    root: {
        height: `${CARD_HEIGHT}px`,
        backgroundPosition: "center",
        backgroundSize: "cover"
    },
    textContainer: {
        height: "100%",
        padding: theme.spacing(1),
        background: "linear-gradient(rgba(0,0,0,0) 35%, rgba(0,0,0,1))"
    },
    textItem: {
        maxWidth: "100%"
    }
}));

/**
 * @param {{autoFocus: boolean, primaryText: string, secondaryText: string, image: string, onClick: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
function Card(props) {
    const ripple = useRef(null);
    const styles = cardStyles();
    const onFocus = (e) => {
        if (window.matchMedia("(hover: none) and (pointer: none)").matches) {
            const offset = e.target.offsetTop - window.innerHeight / 2 + e.target.offsetHeight / 2;
            window.scrollTo({top: offset});
        }
        ripple.current?.start(e);
    }
    const onBlur = (e) => ripple.current?.stop(e);
    return (
        <Grid item xs={CARD_WIDTH_XS} sm={CARD_WIDTH_SM} md={CARD_WIDTH_MD}>
            <Paper className={styles.root}
                   tabIndex={0}
                   style={{backgroundImage: `url(${props.image})`, position: "relative"}}
                   ref={useAutoFocusWhenReady(props.autoFocus)}
                   onClick={props.onClick}
                   onFocus={onFocus}
                   onBlur={onBlur}>
                <Grid container
                      direction="column"
                      justify="flex-end"
                      className={styles.textContainer}>
                    <Grid item className={styles.textItem}>
                        <Text variant="subtitle1" noWrap={true}>{props.primaryText}</Text>
                    </Grid>
                    <Grid item className={styles.textItem}>
                        <Text variant="subtitle2" noWrap={true}>{props.secondaryText}</Text>
                    </Grid>
                </Grid>
                <TouchRipple ref={ripple}/>
            </Paper>
        </Grid>
    );
}

const placeholderStyles = makeStyles(() => ({
    root: {
        height: `${CARD_HEIGHT}px`,
        background: grey["900"],
        backgroundImage: `linear-gradient(to right, ${grey["900"]} 0%, ${grey["800"]} 20%, ${grey["900"]} 40%, ${grey["900"]} 100%)`,
        backgroundRepeat: "no-repeat",
        animation: `$shimmer 1s linear 0s infinite`,
    },
    "@keyframes shimmer": {
        "0%": {
            backgroundPosition: `-${CARD_HEIGHT}px 0`
        },
        "100%": {
            backgroundPosition: `${CARD_HEIGHT}px 0`
        }
    }
}));

function CardPlaceholder() {
    return (
        <Grid item xs={CARD_WIDTH_XS} sm={CARD_WIDTH_SM} md={CARD_WIDTH_MD}>
            <Paper classes={placeholderStyles()}/>
        </Grid>
    );
}

function CardPlaceholders() {
    const greaterThanMd = useMediaQuery(theme => theme.breakpoints.up('md'));
    const greaterThanSm = useMediaQuery(theme => theme.breakpoints.up('sm'));
    let cardWidth;
    if (greaterThanMd) {
        cardWidth = CARD_WIDTH_MD;
    } else if (greaterThanSm) {
        cardWidth = CARD_WIDTH_SM;
    } else {
        cardWidth = CARD_WIDTH_XS;
    }
    const placeholdersToDisplay = 12 / cardWidth * 4;
    const placeholders = [];
    for (let i = 0; i < placeholdersToDisplay; i++) {
        placeholders.push(<CardPlaceholder key={i}/>);
    }
    return placeholders;
}

function CardList(props) {
    const {selected, items, state, loadMoreEvent} = props.cardGrid;
    const loadMore = debounce(LOAD_MORE_DEBOUNCE_INTERVAL, true, () => props.sendEvent(loadMoreEvent));
    useEvent("scroll", () => {
        if (state === "loaded" && window.scrollY + window.innerHeight > document.body.scrollHeight - CARD_HEIGHT) {
            loadMore();
        }
    });
    const cards = [];
    for (let i = 0; i < items.length; i++) {
        const item = items[i];
        cards.push(
            <Card key={i}
                  autoFocus={i === selected}
                  primaryText={item.primaryText}
                  secondaryText={item.secondaryText}
                  image={item.image}
                  onClick={() => props.sendEvent(item.selectEvent)}/>
        );
    }
    return cards;
}

/**
 * @param {{cardGrid: Object, sendEvent: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function CardGrid(props) {
    const {items, state} = props.cardGrid;
    let result;
    if (!state) {
        result = null;
    } else if (state === "loading") {
        result = <CardPlaceholders/>;
    } else if (items.length === 0) {
        result = <EmptyGridPlaceholder placeholderText={props.cardGrid.emptyGridPlaceholderText}/>;
    } else {
        result = <CardList {...props}/>;
    }
    return (
        <Grid container
              spacing={2}>
            {result}
        </Grid>
    );
}