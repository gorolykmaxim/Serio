import React from 'react';
import ReactDOM from 'react-dom';
import './index.css';
import {SerioApplication} from "./SerioApplication";
import SerioController from "./SerioController";
import enableDevelopmentApi from "./DevelopmentApi";
import {parse} from "query-string";
import 'material-icons-font/material-icons-font.css';
import 'roboto-fontface/css/roboto/roboto-fontface.css';
import 'typeface-passion-one';

const DESKTOP_PLATFORM = 0;
const MOBILE_PLATFORM = 1;
const TV_PLATFORM = 2;

const PRODUCTION_RUNTIME = 0;
const DEVELOPMENT_RUNTIME = 1;

const hash = parse(window.location.hash);
window.platform = parseInt(hash.platform || DESKTOP_PLATFORM);
window.runtimeType = parseInt(hash.runtimeType || DEVELOPMENT_RUNTIME);

if (window.runtimeType === DEVELOPMENT_RUNTIME) {
    window.serioController = new SerioController();
    enableDevelopmentApi();
}

ReactDOM.render(
  <React.StrictMode>
      <SerioApplication isTv={window.platform === TV_PLATFORM} isMobile={window.platform === MOBILE_PLATFORM}/>
  </React.StrictMode>,
  document.getElementById('root')
);
