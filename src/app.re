MaterialUi.injectTapEventPlugin ();

type routes =
  | HomeRoute
  | ArticleRoute string;

let router = DirectorRe.makeRouter {"/": "home", "/article/:articleId": "article"};

let renderForRoute route => {
  let element =
    switch route {
    | HomeRoute => <ArticlesOverview router />
    | ArticleRoute articleId => <ArticlePage router articleId />
    };
  Js.log2 "Render route" route;
  ReactDOMRe.renderToElementWithId element "index"
};

let handlers = {
  "home": fun () => renderForRoute HomeRoute,
  "article": fun (articleID: string) => renderForRoute (ArticleRoute articleID)
};

DirectorRe.configure router {"html5history": false, "resource": handlers};

DirectorRe.init router "/";