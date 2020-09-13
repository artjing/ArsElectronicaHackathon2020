let x;
let y;
let ix;
let iy;
let emotionColor = 0;
let emotionResults = [];

let isShowGraphic = 0;

var socket;
var isConnected;

function setup() {
  createCanvas(windowWidth,windowHeight);
  noStroke();
  background(0);
}

function draw() {


  fill(255,0,0,2);

  //console.log(emotionColor)
  if(isShowGraphic == 1)
  {
      if(emotionColor == 0){ 
        fill(0,255,0,2);
      }else if(emotionColor == 1){
        fill(255,0,0,2);
      }else if(emotionColor == 2){
        fill(0,0,255,2);
      }else if(emotionColor == 3){
        fill(255,255,0,2);
      }else if(emotionColor == 4){
        fill(0,255,255,2);
      }else if(emotionColor == 5){
        fill(255,255,255,2);
      }else if(emotionColor == 6){
        fill(255,100,0,2);
      }

  x = mouseX;
  y = mouseY;
  ix = width - mouseX;
  iy = mouseY - height;
  circle(x,height/2,y);
  //fill(0,255,0,2);
  // circle(ix,height/2,iy);
  //fill(0,0,255,2);
  circle(y,height/2,x);
  //fill(255,255,0,2)
  // circle(iy,height/2,ix);
  }else{
    background(0);
  }

}


function touchStarted() {
  if (getAudioContext().state !== 'running') {
    getAudioContext().resume();
  }
}


function setoutEmotionData(e) {
  // send these over OSC to AbletonOSC after you've selected 8 parameters to modify
  if (isConnected) {
    for(var i = 0;i<e.length;i++){
      // console.log(e[i]);
      socket.emit('message', ["/"+e[i].emotion, e[i].val]);
    }
    // socket.emit('message', ['happy', emotions[0]],'angry', emotions[1],'disgusted', emotions[2],'fear', emotions[3],'surprised', emotions[4],'neutral', emotions[5],'sad', emotions[6);
  }
}

function setoutMainEmotion(e){
  if (isConnected) {

    var maxValEmotion = 0;
    var maxIndex = -1;
      for(var i = 0;i<e.length;i++){
        // console.log(e[i]);

        if(e[i].val > maxValEmotion){
          maxIndex = i;
          maxValEmotion = e[i].val;
        }
      }

    if(maxIndex != -1){
      //console.log(maxIndex);
      socket.emit('message', ["/mainEmotion", maxIndex, e[maxIndex].emotion, e[maxIndex].val]);
    }

    }

}

function setoutAllEmotionData(emotion){


      var maxValEmotion = 0;
      var maxIndex = -1;
      var maxValName = '';
      for(var i = 0;i<emotion.length-1;i++){
        if(emotion[i].val > maxValEmotion){
          maxIndex = i;
          maxValEmotion = emotion[i].val;
          emotionColor = i;
        }
      }
      console.log(emotionColor);
}

function receiveOsc(address, value) {
  console.log("received OSC: " + address + ", " + value);
}

function sendOsc(address, value) {
  socket.emit('message', [address, value]);
}

function setupOsc(oscPortIn, oscPortOut) {
  socket = io.connect('http://127.0.0.1:8081', { port: 8081, rememberTransport: false });
  socket.on('connect', function() {
    socket.emit('config', { 
      server: { port: oscPortIn,  host: '127.0.0.1'},
      client: { port: oscPortOut, host: '127.0.0.1'}
    });
  });
  socket.on('connect', function() {
    isConnected = true;
  });
  socket.on('message', function(msg) {
    if (msg[0] == '#bundle') {
      for (var i=2; i<msg.length; i++) {
        receiveOsc(msg[i][0], msg[i].splice(1));
      }
    } else {
      receiveOsc(msg[0], msg.splice(1));
    }
  });
}

function keyPressed() {

  if (keyCode === LEFT_ARROW) {
    isShowGraphic = 1;
  } else if (keyCode === RIGHT_ARROW) {
    isShowGraphic = 0;
  }
}