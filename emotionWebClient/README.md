###set up webclient

**Clone this repo **

	$ git clone https://github.com/pierrextardif/ArsElectronicaHackathon2020.git
	$ cd ArsElectronicaHackathon2020/emotionWebClient
**Mac:**
	$ cp -r ./ /Library/WebServer/Documents
	[http://localhost/face/client/](http://localhost/face/client/)

**Windows:**
	$ python2：
	$ python -m SimpleHTTPServer 8000
	$ python3：
	$ python -m http.server 8000
	[http://127.0.0.1:8000](http://127.0.0.1:8000)

**Run npm and start node:
	$ npm install
	$ node bridge.js
	$ sudo apachectl start
	
Refresh the browser, and open the maxpat or openframework.app to receive OSC data
