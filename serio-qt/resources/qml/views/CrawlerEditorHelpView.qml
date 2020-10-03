import QtQuick 2.12
import QtQuick.Layouts 1.12
import Serio 1.0
import "../widgets"

ViewLayout {
    onCreated: backBtn.clicked.connect(() => actionRouter.trigger(ActionType.BACK, []))
    AccentTitle {
        text: "Help"
    }
    RaisedButton {
        id: backBtn
        focus: true
        primary: false
        text: "back"
    }
    ScrollableColumn {
        Layout.fillWidth: true
        Layout.fillHeight: true
        spacing: globalPadding
        Paragraph {
            width: parent.width
            text: "Crawler is a thing, that goes to an external website and crawls it's content searching for various data (episode videos crawler searches for links to video files sof TV show episodes)."
        }
        Paragraph {
            width: parent.width
            text: "A crawler consists of an array of steps. When a crawler is executed all of it's steps are executed in their specified order. Each step produces an output data, which in turn is getting assigned to the following step as an input data to operate on. Thus, you can treat a crawler as a data transformation pipeline where in the end there should be a link (for thumbnail crawler) or a list of links (for episode video crawler)."
        }
        Paragraph {
            width: parent.width
            text: "The data, that is being passed between the crawler steps, is just an array of strings: each step accepts a string array as its input and produces a string array as its output. First step in each crawler is kind of special because it receives an empty array as an input (episode names crawler is an exception: there the first step receives the list of links to episode video files, crawled by the episode videos crawler)."
        }
        Paragraph {
            width: parent.width
            text: "Thumbnail crawler will use the first string from the result array as a TV show thumbnail image URL. Episode video crawler will treat strings from the result array as episode video links in the order in which they were crawled, meaning the first link will be considered the first episode, the second link - the second episode etc. Episode name crawler will work the same way the episode video crawler does: every string will be used as an episode name of a corresponding episode."
        }
    }
}
