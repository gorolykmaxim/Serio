import Dialog from "../common/Dialog";

export default function create(data, sendEvent) {
    return <Dialog sendEvent={sendEvent} {...data}/>
}