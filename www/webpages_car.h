static const char mainPage[] PROGMEM = u8R"(
  <!DOCTYPE html>
  <html>
  <head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>旗標科技手機遙控自走車</title>
    <style type='text/css'>
      body {
        background: rgb(20, 20, 20);
        background: -webkit-radial-gradient(rgb(100, 100, 100), rgb(50, 50, 50), rgb(20, 20, 20));
        background: -o-radial-gradient(rgb(100, 100, 100), rgb(50, 50, 50), rgb(20, 20, 20));
        background: -moz-radial-gradient(rgb(100, 100, 100), rgb(50, 50, 50), rgb(20, 20, 20));
        background: radial-gradient(rgb(100, 100, 100), rgb(50, 50, 50), rgb(20, 20, 20));
        font-family: Microsoft JhengHei;
      }

      h1 {

        font-size: 22px;
        color: #ffffff;
        text-align: center;
        margin-top: 10px;
        margin-bottom: 5;
      }

      img.phone {
        opacity: 1;
        filter: alpha(opacity=100);
        /* For IE8 and earlier */
      }

      img.flashlight {
        opacity: 0.3;
        filter: alpha(opacity=30);
        /* For IE8 and earlier */
      }

      p.ctlArea {
        margin-bottom: -1px;
        font-size: 13px;
        text-align: center;
        background-color: #09b4b7;
        color: rgb(240, 240, 240);
        width: 67px;
        padding: 4px;
        margin-left: 5px;
        margin-right: auto;
      }

      div.center {
        margin: auto;
        width: 80%;
        border-top: 2px solid gray;
        border-bottom: 2px solid gray;
        padding: 5px;
        text-align: left;
      }

      div.setArea {
        padding: 20px;
        text-align: center;
      }

      .switch {
        position: relative;
        vertical-align: 50%;
        display: inline-block;
        width: 60px;
        height: 34px;
      }

      .switch input {
        display: none;
      }

      .slider {
        position: absolute;
        cursor: pointer;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #f0288c;
        -webkit-transition: .4s;
        transition: .4s;
      }

      .slider:before {
        position: absolute;
        content: "";
        height: 26px;
        width: 26px;
        left: 4px;
        bottom: 4px;
        background-color: white;
        -webkit-transition: .4s;
        transition: .4s;
      }

      input:checked+.slider {
        background-color: #2196F3;
      }

      input:focus+.slider {
        box-shadow: 0 0 1px #2196F3;
      }

      input:checked+.slider:before {
        -webkit-transform: translateX(26px);
        -ms-transform: translateX(26px);
        transform: translateX(26px);
      }

      /* Rounded sliders */
      .slider.round {
        border-radius: 34px;
      }

      .slider.round:before {
        border-radius: 50%;
      }


      #canvas {
        height: 80px;
        width: 100%;
        display: block;
        z-index: -99;
      }

      #joystick-main {
        position: relative;
        display: flex;
        justify-content: center;
        margin: auto;
        z-index: 1;
      }

      #joystick-base {
        bottom: 32px;
        left: 32px;
        height: 250px;
        width: 250px;
        border-radius: 100%;
        border: 10px solid rgba(0, 136, 204, 0.5);
        transition: border-color 0.2s;
        cursor: pointer;
        touch-action: none;
        z-index: 2;
      }

      .joystick-stick {
        position: absolute;
        top: calc(50% - 32px);
        left: calc(50% - 32px);
        height: 64px;
        width: 64px;
        border-radius: 100%;
        background: linear-gradient(to bottom right, rgba(250, 250, 250, 0.8), rgba(5, 120, 180, 0.8), rgba(5, 120, 180, 0.8), rgba(100, 200, 250, 0.8));
        transition: background 0.3s;
        will-change: transform;
        touch-action: none;
        z-index: 3;
      }
    </style>
    <script>
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
      };
      function switchMode() {
        var edArea = document.getElementById('edArea');
        if (document.getElementById('carModeSwitch').checked == false) {
          edArea.style.visibility="hidden";
          ctlArea.innerHTML = '光感應控制程式執行中';
          ctlArea.style.width = '200px';
          ctlArea.style.backgroundColor = '#f0288c';
          document.getElementById('phone').style.opacity = '0.3';
          document.getElementById('flashlight').style.opacity = '1';
          document.getElementById('ctlArea').style.marginLeft = 'auto';
          xhttp.open('GET', 'Race?output=M', true);
          xhttp.send();
        }
        else {
          edArea.style.visibility="visible";
          ctlArea.innerHTML = '手機遙控區';
          ctlArea.style.width = '67px';
          ctlArea.style.backgroundColor = '#09b4b7';
          document.getElementById('phone').style.opacity = '1';
          document.getElementById('flashlight').style.opacity = '0.3';
          document.getElementById('ctlArea').style.marginLeft = '5px';
          xhttp.open('GET', 'Race?output=A', true);
          xhttp.send();
        }

      }

      function initAll() {
        var canvas = document.getElementById("canvas");
        var motorOutLR = [0, 0];
        var motorOutLRnow = [0, 0];
        function LOOP() {
          this.width;
          this.height;
          this.resize();
          this.state = {
            position: {
              x: (this.width / 2),
              y: (this.height / 2)
            },
            velocity: {
              x: 0,
              y: 0
            },
            rotation: 0,
            maxSpeed: 20,

          };

          this.lastRender = 0;
          this.friction = true;
        };

        LOOP.prototype.resize = function () {
          this.width = window.innerWidth * 2;
          this.height = window.innerHeight * 2;
          canvas.width = this.width;
          canvas.height = this.height;
        };

        LOOP.prototype.update = function (progress) {
          var p = progress / 16;
          this.updateMovement(p);
          this.updatePosition(p);
        };

        LOOP.prototype.updateMovement = function (p) {

          var accelerationVector = {
            x: p * 0.1,
            y: p * 0.1
          };

          if (joystick.currentPos.y > 0) {
            this.state.velocity.y += accelerationVector.y;
          };

          if (joystick.currentPos.y < 0) {
            this.state.velocity.y -= accelerationVector.y;
          };

          if (joystick.currentPos.x > 0) {
            this.state.velocity.x += accelerationVector.x;
          };

          if (joystick.currentPos.x < 0) {
            this.state.velocity.x -= accelerationVector.x;
          };


          // friction
          if (this.friction) {
            this.state.velocity.x *= 0.99;
            this.state.velocity.y *= 0.99;
          } else {
            this.state.velocity.x *= 1.00;
            this.state.velocity.y *= 1.00;
          };

          // Limit speed
          if (this.state.velocity.x > this.maxSpeed) {
            this.state.velocity.x = this.maxSpeed;
          }
          else if (this.state.velocity.x < -this.maxSpeed) {
            this.state.velocity.x = -this.maxSpeed;
          };
          if (this.state.velocity.y > this.maxSpeed) {
            this.state.velocity.y = this.maxSpeed;
          }
          else if (this.state.velocity.y < -this.maxSpeed) {
            this.state.velocity.y = -this.maxSpeed;
          };
        }

        LOOP.prototype.updatePosition = function (p) {
          this.state.position.x += this.state.velocity.x;
          this.state.position.y += this.state.velocity.y;

          // Detect boundaries
          if (this.state.position.x > this.width) {
            this.state.position.x -= this.width;
          }
          else if (this.state.position.x < 0) {
            this.state.position.x += this.width;
          }
          if (this.state.position.y > this.height) {
            this.state.position.y -= this.height;
          }
          else if (this.state.position.y < 0) {
            this.state.position.y += this.height;
          };
        };

        LOOP.prototype.main = function (timestamp) {
          var progress = timestamp - this.lastRender;
          this.update(progress);

          this.lastRender = timestamp;
        }


        function pause() {
          cancelAnimationFrame(RAF);
        }

        function countSpeed(motorL, motorR) {
          motorOutLRnow[0] = Math.round(motorL);
          motorOutLRnow[1] = Math.round(motorR);
          if ((motorOutLR[0] != motorOutLRnow[0]) || (motorOutLR[1] != motorOutLRnow[1])) {
            xhttp.open('GET', 'Race?output=' + motorOutLRnow.join("x"), true);
            xhttp.send();
            motorOutLR[0] = motorOutLRnow[0];
            motorOutLR[1] = motorOutLRnow[1];
          }
        }

        function JOYSTICK(parent) {
          this.dragStart = null;
          this.currentPos = { x: 0, y: 0 };
          this.maxDiff = 125;
          this.stick = document.createElement('div');
          this.stick.classList.add('joystick-stick');
          parent.appendChild(this.stick);

          this.stick.addEventListener('mousedown', this.handleMouseDown.bind(this));
          document.addEventListener('mousemove', this.handleMouseMove.bind(this));
          document.addEventListener('mouseup', this.handleMouseUp.bind(this));
          this.stick.addEventListener('touchstart', this.handleMouseDown.bind(this));
          document.addEventListener('touchmove', this.handleMouseMove.bind(this));
          document.addEventListener('touchend', this.handleMouseUp.bind(this));

        };

        function absorbEvent_(event) {
          var e = event || window.event;
          e.preventDefault && e.preventDefault();
          e.stopPropagation && e.stopPropagation();
          e.cancelBubble = true;
          e.returnValue = false;
          return false;
        }

        JOYSTICK.prototype.handleMouseDown = function (event) {
          this.stick.style.transition = '0s';
          if (event.changedTouches) {
            this.dragStart = {
              x: event.changedTouches[0].clientX,
              y: event.changedTouches[0].clientY,
            };
          }
          else {
            this.dragStart = {
              x: event.clientX,
              y: event.clientY,
            };
          }
          return absorbEvent_(event);
        };

        JOYSTICK.prototype.handleMouseMove = function (event) {
          if (this.dragStart === null) return;
          event.preventDefault();
          if (event.changedTouches) {
            event.clientX = event.changedTouches[0].clientX;
            event.clientY = event.changedTouches[0].clientY;
          }
          const xDiff = event.clientX - this.dragStart.x;
          const yDiff = event.clientY - this.dragStart.y;
          const angle = Math.atan2(yDiff, xDiff);
          const distance = Math.min(this.maxDiff, Math.hypot(xDiff, yDiff));
          const speedRatio = distance / this.maxDiff;
          const srcDirCount = Math.round(angle * 3.2);
          const dirCount = Math.abs(srcDirCount);
          if (dirCount > 5) {
            dirCount_out = (dirCount - 10) * (4 - 0) / (6 - 10) + 0;
          }
          else {
            dirCount_out = dirCount;
          }


          if (srcDirCount == -5) {
            //dirQuand = 5;
            countSpeed(speedRatio * 5, speedRatio * 5);
          }
          else if (srcDirCount == 5) {
            //dirQuand = 6;
            countSpeed(-(speedRatio) * 5, -(speedRatio) * 5);
          }
          else {
            if (srcDirCount <= 0 && srcDirCount > -5) {
              //dirQuand = 1;
              countSpeed(speedRatio * 5, speedRatio * dirCount_out);
            }
            else if (srcDirCount > 5 && srcDirCount < 10) {
              //dirQuand = 3;
              countSpeed(-speedRatio * dirCount_out, -speedRatio * 5);
            }
            else if (srcDirCount > 0 && srcDirCount < 5) {
              //dirQuand = 4;
              countSpeed(-speedRatio * 5, -speedRatio * dirCount_out);
            }
            else {
              //dirQuand = 2;
              countSpeed(speedRatio * dirCount_out, speedRatio * 5);
            }
          }
          // Get the distance between the cursor and the center
          const distanceOld = Math.sqrt(Math.pow(xDiff, 2) + Math.pow(yDiff, 2));

          const xNew = distance * Math.cos(angle);
          const yNew = distance * Math.sin(angle);
          this.stick.style.transform = `translate3d(${xNew}px, ${yNew}px, 0px)`;
          this.currentPos = { x: xNew, y: yNew };
          return absorbEvent_(event);
          //console.log( this.currentPos, distance, angle, distanceOld );
        };

        JOYSTICK.prototype.handleMouseUp = function (event) {
          if (this.dragStart === null) return;
          this.stick.style.transition = '.2s';
          this.stick.style.transform = `translate3d(0px, 0px, 0px)`;
          this.dragStart = null;
          this.currentPos = { x: 0, y: 0 };
          countSpeed(0, 0);
          return absorbEvent_(event);
        };

        const joystick = new JOYSTICK(document.getElementById('joystick-base'));
        var loop = new LOOP();
        window.onresize = loop.resize();
        loop.main();
        //# sourceURL=pen.js
      }
    </script>
  </head>
  <body onload='initAll()'>
    <h1>旗標科技手機遙控自走車</h1>
    <div class='center'>
      <!-- 設定區 -->
      <div class='setArea'>
        <img class="flashlight" id="flashlight" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAEAAAABACAQAAAAAYLlVAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAAAmJLR0QAAKqNIzIAAAAJcEhZcwAADdcAAA3XAUIom3gAAAAHdElNRQfiAwcDNRdui6toAAACoUlEQVRo3u2XzUsUYRzHv1u2ggvVIWV7AUG0vIeJ0ZuoGHrtIPgXhLdOdZKC8mbgf+Ap0FNkSCdfCFQiyJOWggZlQQpG7kss9engwzC6s+vsOM8MxP6GgZnf/ng+n2dmn5eRLARX+cwzGy37FXgK7PirPWHF4KQ5YxOoIKoC/6nAD3MeChq5bq0nnHJdn2aYjqKKXnJgRYEaxsnzpGxNDzkgR6MN/CT7MVKyppsskKPXHr5QWoEuskCWHnv4eZpZ8VagiQyQpdsefo6URNpbgT4gQ5c9/Cwpk/FUIEEfTfbwM9S5sulSLyJ8gSEAFtz4SBWYAGCX9qJfolFg04z9eBRoAGCKQmUKdLDGrnPslJ89ywn0A9DGvcoUGOZgbNQENLgm6beWE++QXuiM3tCbWHIXJL7TqRm16iFKPHLSz7WneufujyaDPoFpYNFcV/gUQgm2gTHnLmoFmgAYdGWiVWAAgJYDuSgVGPX68PChEHTQFTW3AaySrEChmQKQd2/cguNXzBh+6VeBFPMAjIeHX/evQIo5ACYJOusU4UdI8sqfAnXMho6XJJK8PlqBOmaOhecs57zwkkQt00cqLBwPf4OfZEiXXFpKKJCkjSlnwQmMv8kvIEt96amEWveLoIVBxlgk71rvAuNvsQfkuVt+JnP+jqvsHFpqN5lgyDLeKKy7sNtM85h+GgKBTZO3/eMlRva3FowyEMr2mzsB8CukQ0AbfCZOfGcA/GrYvc/FhJdYCtD786HhJR7wgZ7Y8KbpKt4f/mMVHyf+QhUfF/6TBbxkdq1x4SXeA3A/JrxEM1+Av94K1vHlFRz8RYt4SaKFr8UKkeG9FQx+LRK8JHHZrRA53ihs7SvEgpckrrDlfGBEjzcK32LESxKtLPOWS1Hx/gElYQOUcCQyMwAAACV0RVh0ZGF0ZTpjcmVhdGUAMjAxOC0wMy0wN1QwMzo1MzoyMyswMTowMHFOBhkAAAAldEVYdGRhdGU6bW9kaWZ5ADIwMTgtMDMtMDdUMDM6NTM6MjMrMDE6MDAAE76lAAAAGXRFWHRTb2Z0d2FyZQB3d3cuaW5rc2NhcGUub3Jnm+48GgAAAABJRU5ErkJggg==">
        <label class="switch">
          <input type="checkbox" id='carModeSwitch' checked onchange='switchMode()'>
          <span class="slider round"></span>
        </label>
        <img class="phone" id="phone" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAEAAAABACAQAAAAAYLlVAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAAAmJLR0QAAKqNIzIAAAAJcEhZcwAADdcAAA3XAUIom3gAAAAHdElNRQfiAwcDNS9GiRP2AAABEUlEQVRo3u3ZMQ4BQRgF4LdLokMnSonGPWSzcYY9gM5lJFxA4wQKiTOolGodW2l4GoVszD9rFtu8V+7MvP9LtvwBM0y4Y87Q5NwxQXg4Cx79mlno+CGvXwFcOXRPaRqCFC3HyR1jHAvfBtgifnu7hRTzEMDIeRJjhbzwre0YbzeZgIZx1kcf5WM0Nb2PTzh/MKqYLnrBb7kkSU4rjAenJMml+0Zcvuw3EUAAAQQQQAABBBBAAAEEEEAAAQQQQAABBBBAAAFqB/g3Jgsu6gVYITYAUkThFdV+QRZNogmyKhUW4OZ5e8EaALDGxXPTaLIAB09t57kPHKHjuelrep8Sq9s9M2bcV1nd2oR6l9fAP9b3D8v1xn6cOJCAAAAAJXRFWHRkYXRlOmNyZWF0ZQAyMDE4LTAzLTA3VDAzOjUzOjQ3KzAxOjAwQ24rjQAAACV0RVh0ZGF0ZTptb2RpZnkAMjAxOC0wMy0wN1QwMzo1Mzo0NyswMTowMDIzkzEAAAAZdEVYdFNvZnR3YXJlAHd3dy5pbmtzY2FwZS5vcmeb7jwaAAAAAElFTkSuQmCC">
      </div>
      <!-- 控制區 -->
      <p class='ctlArea' id='ctlArea'>手機遙控區</p>
      <div  id='edArea'>
        <div id="joystick-main">
          <div id="joystick-base"></div>
        </div>
        <canvas id="canvas" width="1496" height="488" />
      </div>
    </div>
  </body>
  </html>

)";
//----------------------這裡是錯誤路徑頁面--------------------
static const char errorPage[] PROGMEM= u8R"(
  <!DOCTYPE html>
  <html>
  <head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>網站路徑錯誤</title>
  </head>
  <body>
  <h1 align="center">網址錯誤, <a href='/'>請點此跳回主頁</a></h1>
  </body>
  </html>
)";
//----------------------這裡是設定路徑頁面--------------------
static const char settingPage[] PROGMEM= u8R"(
  <!DOCTYPE html>
  <html>
  <head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>設定頁面</title>
  </head>
  <body>
  <h1 align="center">不用設定, <a href='/'>請點此跳回主頁</a></h1>
  </body>
  </html>
)";
//---------以下這一行不要刪除, 這可以讓網頁資料儲存在程式區, 節省記憶體--------------
#define WEBPAGE_IN_PROGMEM
