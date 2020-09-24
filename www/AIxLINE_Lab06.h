﻿//----------------------這裡是主頁面--------------------
static const char mainPage[] PROGMEM = u8R"(
<!DOCTYPE html>
<html>
<head>
 <meta charset="UTF-8">
 <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
 <title>AI 聲控電源</title>
 <link href="https://cdnjs.cloudflare.com/ajax/libs/noty/3.1.4/noty.min.css" rel="stylesheet">
 <style>
  *, *:before, *:after {
   box-sizing: border-box;
   hyphens: auto;
   overflow-wrap: break-word;
  }

  html {
   font-size: 20px;
   min-height: 100%;
  }

  body {
   max-width: 700px;
   min-height: 100%;
   margin: 2em auto;
   padding: 0 0.5em;
   background: linear-gradient(45deg, #2A69AC, #72B7EA);
   background-repeat: no-repeat;
   color: #000;
  }

  h1 {
   text-align: center;
  }

  button, input, select, textarea {
   font-size: inherit;
   color: #000;
   border: 1px solid #333333;
   background: transparent;
   padding: 2px;
  }

  .button {
   padding: 0.5em;
   margin: 1em 0;
   height: 70px;
   width: 100%;
   border-radius: 10px;
   border: none;
   background: #3b5998;
   color: #fff;
  }

  .hidden, [hidden] {
   display: none;
  }

  .log {
   border: 1px solid #333333;
   height: 200px;
   line-height: 1.3em;
   overflow-y: scroll;
   width: 100%;
  }

  .container {
   background: rgba(255,255,255);
   border-radius: 10px;
   border: 1px solid rgba(255,255,255,0.2);
   box-shadow: 0px 0px 10px rgba(255,255,255,0.05);
  }

  #video{
   width: 100%;
  }

  /* === Responsive === */
  .container {
   padding: 10px 60px 10px 60px;
  }

  .button {
   padding: 0.5em;
  }

  #personSelectList,
  #personName {
   width:50%;
  }

  @media (max-width: 768px) {
   .container {
    padding: 10px 30px 10px 30px;
   }
  }
  @media (max-width: 576px) {
   .container {
    padding: 5px 15px 5px 15px;
   }

   .button {
    padding: 0.2em;
   }

   #personSelectList {
    width:100%;
   }
  }
 </style>
</head>
<body>
 <div class="page-ui">
  <div class="container">
   <h1>AI 聲控電源<img id="start_img" src=""></h1>
   <p hidden class="page-error"></p>
   <div class="page-ok">
     <textarea aria-label="Transcription" id="transcription" class="log" readonly></textarea>
     <input type="text" id="open-keywords" name="open-keywords" style="width: 100%;" placeholder="開啟關鍵字, 多個關鍵字請以空白分隔">
     <input type="text" id="close-keywords" name="close-keywords"  style="width: 100%; margin-top: 5px;" placeholder="關閉關鍵字, 多個關鍵字請以空白分隔">
     <button type="button" id="button-play" class="button" style="width: 100%;">開始辨識聲音</button>
    </div>
    <div>
     <button type="button" id="button-stop" class="button" style="width: 100%;">停止辨識聲音</button>
    </div>
   </div>
  </div>
 <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
 <script src="https://cdnjs.cloudflare.com/ajax/libs/noty/3.1.4/noty.min.js"></script>

 <script>
  var restartRecognizer = false;

  if (!window.jQuery) {
   pageError("請先連上網路才能使用 AI 功能");
   throw '';
  };

  Noty.overrideDefaults({
   layout: 'topRight',
   timeout: 5000,
   progressBar: true,
   killer: true,
   visibilityControl: true,
  })

  var startImg = document.getElementById("start_img");
  var buttonPlay = document.getElementById('button-play');
  var buttonStop = document.getElementById('button-stop');

  var mic_gif = 'https://flagtech.github.io/FM611A/mic.gif';
  var mic_animate_gif = 'https://flagtech.github.io/FM611A/mic-animate.gif';

  startImg.src = mic_gif;
  buttonStop.setAttribute('hidden', '');
  
  var synth = window.speechSynthesis;

  function speak(words){
   if (synth.speaking) {
    console.error('speechSynthesis.speaking');
    return;
   }
   if (words) {
    var utterThis = new SpeechSynthesisUtterance(words);
    utterThis.onend = function (event) {
     console.log('SpeechSynthesisUtterance.onend');
    }
    utterThis.onerror = function (event) {
     console.error('SpeechSynthesisUtterance.onerror');
    }
  
    utterThis.lang = "zh-TW";
    utterThis.volume = 0.5;
    utterThis.pitch = 1;
    utterThis.rate = 1;
    synth.speak(utterThis);
   }
  }

  function logEvent(string) {
   console.log(string);
  }

  function recognitionStarted() {
   startImg.src=mic_animate_gif;
   buttonPlay.setAttribute('hidden', '');
   buttonStop.removeAttribute('hidden');
  }

  function recognitionStopped() {
   startImg.src = mic_gif;
   buttonPlay.removeAttribute('hidden');
   buttonStop.setAttribute('hidden', '');
  }

  function pageError(msg) {
   document.querySelector('.page-error').textContent = msg;
   document.querySelector('.page-error').removeAttribute('hidden');
   document.querySelector('.page-ok').setAttribute('hidden', '');
   [].forEach.call(document.querySelectorAll('form button'), function(button) {
    button.setAttribute('disabled', '');
   });
  }

  function ajaxGet(uri) {
   $.ajax(uri)
   .done(function() {
    setTimeout(function(){ Noty.closeAll(); }, 1000);
   })
   .fail(function () {
    new Noty({text:'網路發生錯誤, 無法傳送指令', type:'error'}).show();
   })
  }

  window.SpeechRecognition = window.SpeechRecognition || window.webkitSpeechRecognition || null;
  
  if (!SpeechRecognition) {
   pageError("您的瀏覽器不支援語音功能");
  } else {
   var recognizer = new SpeechRecognition();
   var transcription = document.getElementById('transcription');

   // Start recognising
   recognizer.addEventListener('result', function(event) {
    transcription.textContent = '';
    for (var i = event.resultIndex; i < event.results.length; i++) {
     if (event.results[i].isFinal) {
      var words = event.results[i][0].transcript;
      //transcription.textContent = words + ' (自信度:' + ((event.results[i][0].confidence) * 100).toFixed(2) + '%)';
      transcription.textContent = words;

      var open_keywords = $('#open-keywords').val();
      if (open_keywords == '') open_keywords = '開';
      open_keywords = open_keywords.replace(/ +/g, "|");
      var openRegExp = new RegExp(open_keywords);

      var close_keywords = $('#close-keywords').val();
      if (close_keywords == '') close_keywords = '關';
      close_keywords = close_keywords.replace(/ +/g, "|");
      var closeRegExp = new RegExp(close_keywords);

      if (words.match(openRegExp)) {
       new Noty({text:'語音辨識成功, 正在開啟...', type:'success'}).show();
       ajaxGet("/on");
      }
      else if (words.match(closeRegExp)) {
       new Noty({text:'語音辨識成功, 正在關閉...', type:'success'}).show();
       ajaxGet("/off");
      }
     } else {
      transcription.textContent += event.results[i][0].transcript;
     }
    }
   });

   // Listen for errors
   recognizer.addEventListener('error', function(event) {
    logEvent('Recognition error: ' + event.message);
    //recognitionStopped();
   });

   recognizer.addEventListener('end', function() {
    if (restartRecognizer === true) {
     recognizer.start();
     logEvent('Recognition restarted');
    }
    else {
     logEvent('Recognition ended');
     recognitionStopped();
    }
   });

   buttonPlay.addEventListener('click', function() {
    transcription.textContent = '';

    recognizer.lang = "cmn-Hant-TW";
    recognizer.continuous = true;
    recognizer.interimResults = true;

    try {
     recognizer.start();
     logEvent('Recognition started');
     restartRecognizer = true;
     recognitionStarted();
    } catch(ex) {
     logEvent('Recognition error: ' + ex.message);
     recognitionStopped();
    }
   });

   buttonStop.addEventListener('click', function() {
    recognizer.stop();
    logEvent('Recognition stopped');
    restartRecognizer = false;
    recognitionStopped();
   });
  }
 </script>
</body>
</html>
)";

//----------------------這裡是錯誤路徑頁面--------------------
static const char errorPage[] PROGMEM= u8R"(

)";
  
static const char settingPage[] PROGMEM= u8R"(

)";

//---------勿刪 (讓網頁資料儲存在程式區以節省記憶體)------------
#define WEBPAGE_IN_PROGMEM

