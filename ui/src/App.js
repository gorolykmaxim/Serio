import React from 'react';
import './App.css';
import Button from "./components/Button";
import TextEdit from "./components/TextEdit";
import TextArea from "./components/TextArea";
import {BODY_1, Text} from "./components/Text";

export default function App() {
  return (
      <div>
          <Button text={"learn more"} onClick={() => alert('kekw')}/>
          <br/>
          <TextEdit name={'kekw'} placeholder={'KekW'} disabled defaultValue={"OmegaLULi"}/>
          <br/>
          <TextArea name={'pepega'} placeholder={"Kekw\nMonkaOmega"}/>
          <br/>
          <Text type={BODY_1} primary>Kekw OmegaLUL</Text>
      </div>
  );
}
