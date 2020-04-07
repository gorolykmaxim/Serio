import React from 'react';
import './App.css';
import Button from "./components/Button";
import TextEdit from "./components/TextEdit";
import TextArea from "./components/TextArea";
import {BODY_1, Text} from "./components/Text";
import ListTile from "./components/ListTile";

export default function App() {
  return (
      <div>
          <Button autoFocus={true} text={"learn more"} onClick={() => alert('kekw')}/>
          <br/>
          <TextEdit name={'kekw'} placeholder={'KekW'} disabled defaultValue={"OmegaLULi"}/>
          <br/>
          <TextArea name={'pepega'} placeholder={"Kekw\nMonkaOmega"}/>
          <br/>
          <Text type={BODY_1} primary>Kekw OmegaLUL</Text>
          <br/>
          <ListTile primaryText={'Kekw 1'} secondaryText={'OmegaLUL 1'}/>
          <ListTile primaryText={'Kekw 2'} secondaryText={'OmegaLUL 2'} icon='visibility' onClick={() => alert('pepega')}/>
          <ListTile primaryText={'Kekw 3'} secondaryText={'OmegaLUL 3'}/>
      </div>
  );
}
