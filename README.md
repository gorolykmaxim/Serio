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

### Windows
Execute the install.exe.

If you have vc_redist already installed - just close the vc_redist installation window, that will be opened
at the end of the Serio installation.

## How to build
You need to have Qt5 installed. CMAKE_PREFIX_PATH env variable should be set to Qt's installation location.

Here is the list of useful targets:
- CoreTest - run tests of all the core modules
- SerioTest - run tests of qt implementation of the app
- Serio - run the Qt-based front-end of the application

### MacOS
When you run the Release version of the CMake build - a redistributable .dmg file will be generated in the CMake's 
build directory.

### Windows
In order to build a release version you would need:
- NSIS installed
- NSIS_PATH env variable should be set pointing to the NSIS's installation folder

When you run the Release version of the CMake build - an "output" folder will get generated inside the CMake's build
directory. That output directory will contain the executable and all the necessary dlls to run it.

In case NSIS_PATH env variable has been set before running the build - the output directory will also have an executable 
installer generated. NSIS_PATH is not mandatory to build the release version: if it is not set - the installer won't be
created but the executable and dlls will be.

### Android
In order to build you need following environment variables:
- ANDROID_NDK
- JAVA_HOME
- ANDROID_SDK

and following CMake properties set in the command line:
- ANDROID_NATIVE_API_LEVEL=28
- ANDROID_NDK
- CMAKE_TOOLCHAIN_FILE
- ANDROID_ABI
- ANDROID_STL=c++_shared
- CMAKE_FIND_ROOT_PATH=path to QT

Target to build .apk - SerioApk

At this point you can only generate a single .apk per ABI.