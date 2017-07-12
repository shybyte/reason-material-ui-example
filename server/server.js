const path = require('path')
const jsonServer = require('json-server');
const server = jsonServer.create();
const router = jsonServer.router('server/data.json');
const conf = { static: path.join(process.cwd(), "src") };
console.log(conf);
const middlewares = jsonServer.defaults(conf);
const fs = require('fs');
const port = process.env.PORT || 8888;

server.use(middlewares);
server.use(jsonServer.bodyParser)


server.delete('/articles/:id', (req, res, next) => {
  const articleId = req.params.id;
  const article = router.db.get('articles').find({ id: articleId }).value();
  if (!article) {
    res.sendStatus(404);
    return;
  }

  if (article.default) {
    res.sendStatus(405);
    return;
  }

  next();
})

server.use(router);

server.listen(port, () => {
  console.log('json-server is running!');
});