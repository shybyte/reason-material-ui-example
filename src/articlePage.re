let se = ReasonReact.stringToElement;


type componentState = {article: option Model.article};

let handleArticleLoaded article {ReasonReact.state: _state} =>
  ReasonReact.Update {article: Some article};

let component = ReasonReact.statefulComponentWithRetainedProps "ArticlePage";

let make ::router ::articleId _children => {
  let reload {ReasonReact.update: update} => {
    Webservice.fetchArticle articleId |>
    Js.Promise.then_ (
      fun articles => {
        (update handleArticleLoaded) articles;
        Js.log articles;
        Js.Promise.resolve ()
      }
    ) |> ignore;
    ReasonReact.NoUpdate
  };
  {
    ...component,
    initialState: fun () => {article: None},
    didMount: fun self => reload self,
    retainedProps: articleId,
    willReceiveProps: fun oldSelf =>
      if (articleId !=  oldSelf.retainedProps) {
        {article: None}
      } else {
        oldSelf.state
      },
  didUpdate: fun {oldSelf, newSelf} =>
    if (oldSelf.retainedProps !== newSelf.retainedProps) {
      reload newSelf |> ignore;
    },      
    render: fun self => {
      let gotoHome event => {
        ReactEventRe.Mouse.preventDefault event;
        DirectorRe.setRoute router "/"
      };
      <div>
        (
          switch self.state.article {
          | Some article => <h1> (se article.title) </h1>
          | None => (se ("Loading article " ^ articleId))
          }
        )
        <div> <a href="#" onClick=gotoHome> (ReasonReact.stringToElement "Home") </a> </div>
      </div>
    }
  }
};