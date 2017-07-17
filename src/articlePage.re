let se = ReasonReact.stringToElement;

open MaterialUi;

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
  let onTitleChanged newValue {ReasonReact.state: state} =>
    ReasonReact.Update {
      article: state.article |> Option.map (fun a => {...a, Model.title: newValue})
    };
  let onTextChanged newValue {ReasonReact.state: state} =>
    ReasonReact.Update {
      article: state.article |> Option.map (fun a => {...a, Model.text: newValue})
    };
  let saveArticle _ (self: ReasonReact.self componentState 'a) => {
    self.state.article |>
    Option.map (
      fun (article: Model.article) => Webservice.save article.id article.title article.text
    ) |> ignore;
    ReasonReact.NoUpdate
  };
  {
    ...component,
    initialState: fun () => {article: None},
    didMount: fun self => reload self,
    retainedProps: articleId,
    willReceiveProps: fun oldSelf =>
      if (articleId != oldSelf.retainedProps) {
        {article: None}
      } else {
        oldSelf.state
      },
    didUpdate: fun {oldSelf, newSelf} =>
      if (oldSelf.retainedProps !== newSelf.retainedProps) {
        reload newSelf |> ignore
      },
    render: fun self => {
      let gotoHome event => {
        ReactEventRe.Mouse.preventDefault event;
        DirectorRe.setRoute router "/"
      };
      <MuiThemeProvider>
        <div>
          <div> <a href="#" onClick=gotoHome> (ReasonReact.stringToElement "Home") </a> </div>
          (
            switch self.state.article {
            | Some article =>
              <div>
                <h1> (se article.title) </h1>
                (
                  article.default ?
                    <div> <LockIcon className="icon" /> (se "This article is readonly.") </div> :
                    ReasonReact.nullElement
                )
                <div>
                  <TextField
                    autoFocus=true
                    disabled=article.default
                    value=article.title
                    onChange=(self.update onTitleChanged)
                    floatingLabelText="Article Title"
                  />
                </div>
                <div>
                  <TextField
                    disabled=article.default
                    value=article.text
                    onChange=(self.update onTextChanged)
                    multiLine=true
                    floatingLabelText="Article Text"
                    rows=4
                  />
                </div>
                <RaisedButton
                  disabled=article.default
                  label="Save Article"
                  onClick=(self.update saveArticle)
                />
              </div>
            | None => se ("Loading article " ^ articleId)
            }
          )
        </div>
      </MuiThemeProvider>
    }
  }
};