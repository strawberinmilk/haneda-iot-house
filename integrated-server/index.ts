'use strict';

import puppeteer from 'puppeteer-core';
import express from 'express';
const app = express();
import dotenv from 'dotenv';
dotenv.config();

import { VoiceMatch } from './lib/VoiceMatch';
const voiceMatch = new VoiceMatch();

(async () => {
  app.use(express.static('./view'));
  app.listen(process.env.PORT);

  const browser = await puppeteer.launch({ 
    headless: false,
    executablePath: process.env.CHROME_PATH,
    args:[
      '--use-fake-ui-for-media-stream',
      //'--window-size=0,0',
      //'--window-position=0,0'
    ]
  });
  const page = (await browser.pages())[0];
  await page.exposeFunction('forNode', (result:{status:string, text:string}) => {
    voiceMatch.getData(result);
  });
  await page.goto(`http://127.0.0.1:${process.env.PORT}`);
})();
