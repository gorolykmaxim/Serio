import React from 'react';
import './App.css';
import Button from "./components/Button";
import TextEdit from "./components/TextEdit";

export default function App() {
  return (
      <div>
        <Button text={"learn more"} onClick={() => alert('kekw')}/>
        <br/>
        <TextEdit name={'kekw'} placeholder={'KekW'} className={'kekw'}/>
      </div>
  );
}
