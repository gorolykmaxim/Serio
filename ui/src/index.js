import React from 'react';
import ReactDOM from 'react-dom';
import './index.css';
import {SerioApplication} from "./SerioApplication";
import SerioController from "./SerioController";
import enableDevelopmentApi from "./DevelopmentApi";

const DESKTOP_PLATFORM = 0;
const MOBILE_PLATFORM = 1;
const TV_PLATFORM = 2;

const PRODUCTION_RUNTIME = 0;
const DEVELOPMENT_RUNTIME = 1;

let hash = window.location.hash;
hash = hash ? hash.substr(1, hash.length - 1) : "01";
window.platform = parseInt(hash[0]);
window.runtimeType = parseInt(hash[1]);

if (window.runtimeType === DEVELOPMENT_RUNTIME) {
    window.serioController = new SerioController();
    enableDevelopmentApi();
}

ReactDOM.render(
  <React.StrictMode>
      <SerioApplication isTv={window.platform === TV_PLATFORM}/>
  </React.StrictMode>,
  document.getElementById('root')
);
