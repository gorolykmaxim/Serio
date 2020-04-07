import React from 'react';
import ReactDOM from 'react-dom';
import './index.css';
import AllShows from "./views/AllShows";

const shows = [
    {
        name: 'Boruto',
        thumbnail: 'https://dw9to29mmj727.cloudfront.net/properties/2016/1363-SeriesThumbnail_BorutoAnime_400x320_v2.jpg'
    },
    {
        name: 'One Punch Man',
        thumbnail: 'https://cadelta.ru/images/One-punch_man1.jpg'
    },
    {
        name: 'The Office',
        thumbnail: 'https://img.nbc.com/sites/nbcunbc/files/images/2016/1/19/MDot-TheOffice-640x360-MP.jpg'
    },
    {
        name: 'Mandalorian',
        thumbnail: 'https://cdn.mos.cms.futurecdn.net/SRxSp8y2pSBjCRXo5jiWrG.jpg'
    },
    {
        name: 'Boruto',
        thumbnail: 'https://dw9to29mmj727.cloudfront.net/properties/2016/1363-SeriesThumbnail_BorutoAnime_400x320_v2.jpg'
    },
    {
        name: 'One Punch Man',
        thumbnail: 'https://cadelta.ru/images/One-punch_man1.jpg'
    },
    {
        name: 'The Office',
        thumbnail: 'https://img.nbc.com/sites/nbcunbc/files/images/2016/1/19/MDot-TheOffice-640x360-MP.jpg'
    },
    {
        name: 'Mandalorian',
        thumbnail: 'https://cdn.mos.cms.futurecdn.net/SRxSp8y2pSBjCRXo5jiWrG.jpg'
    },
    {
        name: 'Boruto',
        thumbnail: 'https://dw9to29mmj727.cloudfront.net/properties/2016/1363-SeriesThumbnail_BorutoAnime_400x320_v2.jpg'
    },
    {
        name: 'One Punch Man',
        thumbnail: 'https://cadelta.ru/images/One-punch_man1.jpg'
    },
    {
        name: 'The Office',
        thumbnail: 'https://img.nbc.com/sites/nbcunbc/files/images/2016/1/19/MDot-TheOffice-640x360-MP.jpg'
    },
    {
        name: 'Mandalorian',
        thumbnail: 'https://cdn.mos.cms.futurecdn.net/SRxSp8y2pSBjCRXo5jiWrG.jpg'
    },
];

const showInfo = {
    lastWatchedShows: shows.slice(0, 2),
    allShows: shows
};

ReactDOM.render(
  <React.StrictMode>
    <AllShows showInfo={showInfo} onShowSelect={(show) => alert(JSON.stringify(show))}/>
  </React.StrictMode>,
  document.getElementById('root')
);
