import {Grid, makeStyles, Paper} from "@material-ui/core";
import Text from "./Text";
import TouchRipple from "@material-ui/core/ButtonBase/TouchRipple";
import {useRef} from "react";
import {useAutoFocusWhenReady} from "../Focus";

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
        height: "200px",
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
        <Grid item xs={6} sm={4} md={2}>
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

/**
 * @param {{cardGrid: Object, sendEvent: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function CardGrid(props) {
    const {selected, items, emptyGridPlaceholderText} = props.cardGrid;
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
    if (cards.length === 0) {
        cards.push(
            <EmptyGridPlaceholder placeholderText={emptyGridPlaceholderText}/>
        );
    }
    return (
        <Grid container
              spacing={2}>
            {cards}
        </Grid>
    );
}