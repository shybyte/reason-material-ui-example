const jsonServer = require('json-server');
const server = jsonServer.create();
const router = jsonServer.router('server/data.json');
const middlewares = jsonServer.defaults();
const fs = require('fs');
const port = process.env.PORT || 3000;

server.use(middlewares);
server.use(router);

server.listen(port, () => {
  console.log('json-server is running!');
});