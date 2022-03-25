'use strict';
import axios from 'axios';

const actionList = [
  {text: ['電気', 'つけて'], url: '/ir/lvon'},
  {text: ['電気', '消して'], url: '/ir/lvoff'},
  {text: ['机', 'つけて'], url: '/desk/on'},
  {text: ['机', '消して'], url: '/desk/off'},
  {text: ['デスク', 'つけて'], url: '/desk/on'},
  {text: ['デスク', '消して'], url: '/desk/off'},
];

export class VoiceMatch {
  private flag = '';
  public async getData (data:{status:string, text:string}) {
    if (data.status.match(/(end)|(error)|(reset)/)) {
      this.flag = '';
    }
    const url = this.match(data.text);
    if (url === '') return;
    if (this.flag != url) {
      await axios.get(`${process.env.ESP_URL}${url}`);
      this.flag = url;
    }
  }

  private match (text: string): string {
    for (const action of actionList) {
      let actionHitFlag = true;
      for (const phrase of action.text) {
        if (!text.match(RegExp(phrase))) {
          actionHitFlag = false;
          break;
        }
      }
      if (actionHitFlag) {
        return action.url;
      }
    }
    return '';
  }
}
