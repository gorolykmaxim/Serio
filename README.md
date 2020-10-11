# Serio
Cross-platform application to crawl and watch tv shows.

## How to install

### Mac OS
Open the .dmg file and drag the Serio.app to your /Applications folder.

If you are on Catalina - run following command before launching the app for
the first time:
```
sudo xattr -d com.apple.quarantine /Applications/Serio.app/Contents/MacOS/Serio
```

## How to build
You need to have Qt5 installed. CMAKE_PREFIX_PATH env variable should be set to Qt's installation location.

Here is the list of useful targets:
- CoreTest - run tests of all the core modules
- SerioTest - run tests of qt implementation of the app
- Serio - run the Qt-based front-end of the application