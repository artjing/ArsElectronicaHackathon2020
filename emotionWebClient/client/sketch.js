let colors = [[ '#ff7c7c', '#111' ],[ '#f1ff7c', '#28292b' ],[ '#7cff7e', '#e6e6e6' ],[ '#7c89ff', '#1666bd' ],[ '#ffc67c', '#26282a' ],[ '#7cfff4', '#e24c68' ],[ '#d97cff', '#e55256' ],[ '#af7644', '#fff' ],[ '#44af7c', '#1790d0' ],[ '#cea867', '#fff' ]];

let count = 0;
let old_count = 0;
let startTime;
let my_interval = 700;
let old_millis;
var Y_AXIS = 1;
var X_AXIS = 3;
var from;
var to;
var from_next;
var to_next;

var socket;
var isConnected;

let portEmotionSending = 6665;

function setup() {
  createCanvas(windowWidth, windowHeight);
  //background(color(colors[count][0]));
	old_millis = millis();
	from = color(colors[count][0]);
  from_next = color(colors[(count+1)%10][1]);

  to = color(colors[(count+1)%10][0]);
  to_next = color(colors[(count+2)%10][0]);

  setupOsc(8338, portEmotionSending);

}


function draw(){
background(240, 220, 100);

// if ((((millis()-old_millis))/my_interval)>=1){
// count = (count + 1)%10;
// old_millis = millis();

// }
	
// from = color(colors[count][0]);
// from_next = color(colors[(count+1)%10][1]);

// to = color(colors[(count+1)%10][0]);
// to_next = color(colors[(count+2)%10][0]);
//   setGradient(0, 0, width, height, color(lerpColor(from, to, ((millis()-old_millis))/my_interval)), color(lerpColor(to, to_next, ((millis()-old_millis))/my_interval)), Math.round(Math.random()));

//   // send these over OSC to AbletonOSC after you've selected 8 parameters to modify
//   if (isConnected) {
//     //socket.emit('message', ['/wek/outputs', '1', '2', '3', '4']);
//   }

}

function resetColor()
{

  Tone.Master.volume.value = -2;
  synth = new Tone.Synth({
    oscillator : {
      type: 'sine'
    }
  });
  synth.triggerAttackRelease("A3" , '8n');
  setGradient(0, 0, width, height, color(lerpColor(from, to, ((millis()-old_millis))/my_interval)), color(lerpColor(to, to_next, ((millis()-old_millis))/my_interval)), Math.round(Math.random()));
}



function setGradient(x, y, w, h, c1, c2, axis) {

  noFill();

  if (axis == Y_AXIS) {  // Top to bottom gradient
    for (var i = y; i <= y+h; i++) {
      var inter = map(i, y, y+h, 0, 1);
      var c = lerpColor(c1, c2, inter);
      stroke(c);
      line(x, i, x+w, i);
    }
  }  
  else if (axis == X_AXIS) {  // Left to right gradient
    for (var i = x; i <= x+w; i++) {
      var inter = map(i, x, x+w, 0, 1);
      var c = lerpColor(c1, c2, inter);
      stroke(c);
      line(i, y, i, y+h);
    }
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
      socket.emit('message', [e[i].emotion, e[i].val]);
    }
  }
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