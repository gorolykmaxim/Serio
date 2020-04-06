import React from 'react';
import './App.css';
import Button from "./components/Button";

export default function App() {
  return (
    <Button text={"learn more"} onClick={() => alert('kekw')}/>
  );
}
