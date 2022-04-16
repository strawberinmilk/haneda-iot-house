'use strict';
import axios from 'axios';

const actionList = [
  {text: ['電気', 'つけて'], url: '/ir/lvon'},
  {text: ['電気', '消して'], url: '/ir/lvoff'},
  {text: ['電気', '明るくして'], url: '/ir/lvup'},
  {text: ['電気', '暗くして'], url: '/ir/lvdown'},
  {text: ['常夜灯', 'つけて'], url: '/ir/lvnightlamp'},
  {text: ['机', 'つけて'], url: '/desk/on'},
  {text: ['机', 'けして'], url: '/desk/on'},
  {text: ['デスク', 'つけて'], url: '/desk/on'},
  {text: ['デスク', '消して'], url: '/desk/off'},
];

export class VoiceMatch {
  private sentList: string[] = [];

  public async getData (data:{status:string, text:string}) {
    if (data.status.match(/(end)|(error)|(reset)/)) {
      this.sentList = [];
    }
    const urlList: string[] = this.match(data.text);
    for (const url of urlList) {
      if (this.sentList.indexOf(url) === -1) {
        await axios.get(`${process.env.ESP_URL}${url}`);
        this.sentList.push(url);
      }
    }
  }

  private match (text: string): string[] {
    const actionUrlList: string[] = []
    for (const action of actionList) {
      let actionHitFlag = true;
      for (const phrase of action.text) {
        if (!text.match(RegExp(phrase))) {
          actionHitFlag = false;
          break;
        }
      }
      if (actionHitFlag) {
        actionUrlList.push(action.url)
      }
    }
    return actionUrlList;
  }
}
