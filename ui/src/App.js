import React from 'react';
import './App.css';
import Button from "./components/Button";
import TextEdit from "./components/TextEdit";
import TextArea from "./components/TextArea";

export default function App() {
  return (
      <div>
        <Button text={"learn more"} onClick={() => alert('kekw')}/>
        <br/>
        <TextEdit name={'kekw'} placeholder={'KekW'}/>
        <br/>
        <TextArea name={'pepega'} placeholder={"Kekw\nMonkaOmega"}/>
      </div>
  );
}
