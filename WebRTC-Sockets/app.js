const express = require("express");
const app = new express();
const http = require("http").createServer(app);
const io = require("socket.io")(http);
const path = require("path");


app.use(express.static('/static'));

app.get('/', (req, res) => {
  res.sendFile(path.resolve(__dirname, 'static', 'index.html'));
});

io.on('connection', (socket) => {
  console.log('a user connected');
});

http.listen(3000, () => {
  console.log('listening on *:3000');
});