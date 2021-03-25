import {Grid, makeStyles, Paper} from "@material-ui/core";
import Text from "./Text";
import TouchRipple from "@material-ui/core/ButtonBase/TouchRipple";
import {useRef} from "react";
import {useAutoFocusWhenReady} from "../Focus";

const tvShowCardStyles = makeStyles(theme => ({
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
 * @param {{autoFocus: boolean, title: string, lastWatched: string, thumbnail: string, onClick: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
function TvShowCard(props) {
    const ripple = useRef(null);
    const styles = tvShowCardStyles();
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
                   style={{backgroundImage: `url(${props.thumbnail})`, position: "relative"}}
                   ref={useAutoFocusWhenReady(props.autoFocus)}
                   onClick={props.onClick}
                   onFocus={onFocus}
                   onBlur={onBlur}>
                <Grid container
                      direction="column"
                      justify="flex-end"
                      className={styles.textContainer}>
                    <Grid item className={styles.textItem}>
                        <Text variant="subtitle1" noWrap={true}>{props.title}</Text>
                    </Grid>
                    <Grid item className={styles.textItem}>
                        <Text variant="subtitle2" noWrap={true}>{props.lastWatched}</Text>
                    </Grid>
                </Grid>
                <TouchRipple ref={ripple}/>
            </Paper>
        </Grid>
    );
}

/**
 * @param {{selected: number, tvShows: Array, onSelect: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function TvShowGrid(props) {
    const cards = [];
    for (let i = 0; i < props.tvShows.length; i++) {
        const tvShow = props.tvShows[i];
        cards.push(
            <TvShowCard key={i}
                        autoFocus={i === props.selected}
                        title={tvShow.title}
                        lastWatched={tvShow.lastWatched}
                        thumbnail={tvShow.thumbnail}
                        onClick={() => props.onSelect(tvShow.selectTvShowEvent)}/>
        );
    }
    return (
        <Grid container
              spacing={2}>
            {cards}
        </Grid>
    );
}