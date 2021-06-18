import 'package:flutter/material.dart';

void main() {
  runApp(HelloWorld());
}

class HelloWorld extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: "Hello World",
      theme: ThemeData(
        canvasColor: Colors.black,
        textTheme: TextTheme(
          headline1: TextStyle(color: Colors.orange)
        )
      ),
      home: TitleScreen()
    );
  }
}

class TitleScreen extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Center(
          child: Text(
              "Serio",
              style: Theme.of(context).textTheme.headline1.apply(fontFamily: "PassionOne")
          )
      ),
    );
  }
}