const path = require('path')
const jsonServer = require('json-server');
const server = jsonServer.create();
const router = jsonServer.router('server/data.json');
const Ajv = require('ajv');

const conf = { static: path.join(process.cwd(), "src") };
const middlewares = jsonServer.defaults(conf);
const port = process.env.PORT || 8888;
const ajv = new Ajv(); // options can be passed, e.g. {allErrors: true}

const validateArticle = ajv.compile(
  {
    "title": "Article",
    "type": "object",
    "properties": {
        "title": {
            "type": "string"
        },
        "default": {
            "type": "boolean"
        },
    },
    "required": ["title", "default"]
});

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
});

server.post('/articles', (req, res, next) => {
  const valid = validateArticle(req.body);
  if (!valid) {
    res.status(400);
    res.json({errors: validateArticle.errors});
    return;
  }
  next();
})

server.use(router);

server.listen(port, () => {
  console.log('json-server is running!');
});