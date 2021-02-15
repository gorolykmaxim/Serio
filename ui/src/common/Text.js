import {makeStyles, Typography} from "@material-ui/core";

const nonSelectable = makeStyles(() => ({
    root: {
        userSelect: "none",
        cursor: "default"
    }
}));

/**
 * @param {{
 * align: ('inherit'|'left'|'center'|'right'|'justify'),
 * children: JSX.Element,
 * classes: Object,
 * color: ('initial'|'inherit'|'primary'|'secondary'|'textPrimary'|'textSecondary'|'error'),
 * component: string,
 * display: ('initial'|'block'|'inline'),
 * gutterBottom: boolean,
 * noWrap: boolean,
 * paragraph: boolean,
 * variant: ('h1'|'h2'|'h3'|'h4'|'h5'|'h6'|'subtitle1'|'subtitle2'|'body1'|'body2'|'caption'|'button'|'overline'|'srOnly'|'inherit'),
 * variantMapping: Object
 * }} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function Text(props) {
    let {className, ...other} = props;
    className = `${nonSelectable().root} ${className || ''}`;
    return <Typography className={className} {...other}>{props.children}</Typography>
}