# Desktop port of Serio

Hosts the UI a JavaFX WebView. Stores data in an SQLite database (located in ~/serio.db). 
Uses Spring WebClient to crawl websites.

Works well most of the time, but there are some known issues, that are
not likely to be addressed in the future:
- the application might hang for a minute if you exit it while it is playing
an episode of a show: this is related to the JavaFX's WebView implementation
- some video files might cause the application to crash with a core dump.
Again, it is related to JavaFX's WebView
- if you are using a modern or at least a sort-of-modern version of java
to run the application - the episode player might be loading the episodes'
videos very slowly. Once again, it is related to JavaFX's WebView. This
problem can be avoided by using an older version of JVM, such as "1.8.0_171".