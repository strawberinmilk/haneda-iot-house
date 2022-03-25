if ('SpeechRecognition' in window) {
  console.log('いけるよ');
} else {
  console.log('だめだよ');
}

const speechFn = () => {
  window.forNode({
    status: 'reset',
    text:''
  });

  SpeechRecognition = webkitSpeechRecognition || SpeechRecognition;
  const recognition = new SpeechRecognition();
  
  let oldOnresultTime = null;
  recognition.onresult = (event) => {
    oldOnresultTime = (new Date).getTime();
    const text = event.results[0][0].transcript;
    document.getElementById('text').innerText = text;
    window.forNode({
      status: event.results[0].isFinal ? 'end' : 'talking',
      text
    });
    if (event.results[0].isFinal) {
      speechFn();
    }
  };

  setInterval( () => {
    if (oldOnresultTime===null) return;
    if (8000 < (new Date).getTime() - oldOnresultTime) {
      location.reload();
    };
  }, 1000);

  recognition.onsoundstart = function () {
    document.getElementById('status').innerHTML = '認識中';
  };
  recognition.onnomatch = function () {
    document.getElementById('status').innerHTML = 'ヒットなし';
  };
  recognition.onerror = function (e) {
    console.log(e);
    document.getElementById('status').innerHTML = 'エラー';
    speechFn();
  };
  recognition.onsoundend = function () {
    document.getElementById('status').innerHTML = '認識終了';
    speechFn();
  };
  
  recognition.lang = 'ja-JP';
  recognition.interimResults = true;
  recognition.continuous = true;
  recognition.start();
};
speechFn();
