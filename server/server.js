const path = require('path')
const jsonServer = require('json-server');
const server = jsonServer.create();
const router = jsonServer.router('server/data.json');
const conf = {static: path.join(process.cwd(), "src")};
console.log(conf);
const middlewares = jsonServer.defaults(conf);
const fs = require('fs');
const port = process.env.PORT || 3000;

server.use(middlewares);
server.use(router);

server.listen(port, () => {
  console.log('json-server is running!');
});