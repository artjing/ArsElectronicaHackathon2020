# ArsElectronicaHackathon2020

This is the result of our team   **Humans x AI x SOUND** for the [Ars Electronica Hackathon 2020](https://ars.electronica.art/keplersgardens/en/aixmusic-hackathon/).

## Description

**Face tracking emotions into music**

This project aims to explore the ralationship between emotion and music. AI is an extension of human's intelligence, it acting as a new media, a tool, a collaborator. We creating AI, but it will in turn reshape us. It is like a loop. How far are we from AI decoding our emotion, even consciousness. We want to use music and sound to explore this question.



![.](./Assets/1.png)


## Structure

---
->Using webclient to track emotions in realtime.  

->sending emotion data to openframework by OSC to generate visual.  

->sending OSC data to max/msp to trigger sound.  


## Getting Started

---
### OF
Add faceEmotion folder to your `OF/apps` folder.
Add folders inside local_addons to your `OF/addons` folder.

#### Controls

```cpp
key == 'g' // ==> show hide the GUI for emotions
key == 'h' // ==> show / hide mouse
key == LEFT_ARROW // ==> show / hide content
key == RIGHT_ARROW // ==> show / hide content
key == 'r' // sends the recording message to maxMSP and flashes screen red
```

### WebClienbt & OSC sending

**Clone this repo**

	$ git clone https://github.com/pierrextardif/ArsElectronicaHackathon2020.git
	$ cd ArsElectronicaHackathon2020/emotionWebClient

**Mac:**

	$ cp -r ./ /Library/WebServer/Documents
	$ sudo apachectl start
[http://localhost/face/client/](http://localhost/face/client/)

**Windows:**

	$ python2：
	$ python -m SimpleHTTPServer 8000
	$ python3：
	$ python -m http.server 8000
	
[http://127.0.0.1:8000](http://127.0.0.1:8000)

**Run npm and start node:**

	$ npm install
	$ node bridge.js

Refresh the browser, and open the maxpat or openframework.app to receive OSC data

### MAX/MSP

Open the emotion_mixer.maxpat in maxPatches folder.


### The Magenta.js part will coming soon...




