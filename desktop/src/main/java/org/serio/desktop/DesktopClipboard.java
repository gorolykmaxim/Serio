package org.serio.desktop;

import org.serio.core.clipboard.Clipboard;

import java.awt.*;
import java.awt.datatransfer.StringSelection;

public class DesktopClipboard implements Clipboard {
    @Override
    public void setContent(String content) {
        Toolkit.getDefaultToolkit().getSystemClipboard().setContents(new StringSelection(content), null);
    }
}
