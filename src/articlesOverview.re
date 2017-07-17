let se = ReasonReact.stringToElement;

open MaterialUi;

type articleDialogState =
  | ADD
  | RENAME Model.article
  | ARCTICLE_DIALOG_CLOSED;

type appState = {
  openedDialog: articleDialogState,
  articles: option (array Model.article)
};

let handleArticlesLoaded articles {ReasonReact.state: state} =>
  ReasonReact.Update {...state, articles: Some articles};

let component = ReasonReact.statefulComponent "ArticlesOverview";

let make ::router _children => {
  let toggleDialog newArticleDialogType _event {ReasonReact.state: state} =>
    ReasonReact.Update {...state, openedDialog: newArticleDialogType};
  let reload {ReasonReact.update: update} => {
    Webservice.fetchArticles () |>
    Js.Promise.then_ (
      fun articles => {
        (update handleArticlesLoaded) articles;
        Js.log articles;
        Js.Promise.resolve ()
      }
    ) |> ignore;
    ReasonReact.NoUpdate
  };
  let addArticle title self => {
    Webservice.create title |>
    Js.Promise.then_ (
      fun _result => {
        reload self |> ignore;
        Js.Promise.resolve ()
      }
    ) |> ignore;
    ReasonReact.Update {...self.state, openedDialog: ARCTICLE_DIALOG_CLOSED}
  };
  let renameArticle newTitle (self: ReasonReact.self appState 'a) => {
    switch self.state.openedDialog {
    | RENAME article =>
      Webservice.rename article.id newTitle |>
      Js.Promise.then_ (
        fun _result => {
          reload self |> ignore;
          Js.Promise.resolve ()
        }
      ) |> ignore
    | _ => Js.log2 "Expected RENAME state but got" self.state
    };
    ReasonReact.Update {...self.state, openedDialog: ARCTICLE_DIALOG_CLOSED}
  };
  {
    ...component,
    initialState: fun () => {openedDialog: ARCTICLE_DIALOG_CLOSED, articles: None},
    didMount: fun self => reload self,
    render: fun self => {
      let renderMenuButton (article: Model.article) => {
        let iconButton = <IconButton> <MoreVertIcon className="iconHover" /> </IconButton>;
        <IconMenu iconButtonElement=iconButton>
          <MenuItem
            primaryText="Edit"
            onTouchTap=(fun () => DirectorRe.setRoute router ("/article/" ^ article.id))
          />
          <MenuItem
            primaryText="Copy"
            onTouchTap=(
              fun () => {
                Js.log ("Copy " ^ article.title);
                Webservice.create article.title |>
                Js.Promise.then_ (
                  fun _result => {
                    reload self |> ignore;
                    Js.Promise.resolve ()
                  }
                ) |> ignore
              }
            )
          />
          <MenuItem
            primaryText="Rename"
            disabled=article.default
            title=(article.default ? "This article is readonly. You can't rename it" : "")
            rightIcon=(article.default ? <LockIcon/> : ReasonReact.nullElement)
            onTouchTap=(self.update (toggleDialog (RENAME article)))
          />
          <MenuItem
            primaryText="Delete"
            disabled=article.default
            title=(
              article.default ? "This article is readonly. You can't delete it." : "Are you sure?"
            )
            rightIcon=(article.default ? <LockIcon/> : ReasonReact.nullElement)
            onTouchTap=(
              fun () => {
                Js.log ("Delete " ^ article.title);
                Webservice.delete article.id |>
                Js.Promise.then_ (
                  fun _result => {
                    reload self |> ignore;
                    Js.Promise.resolve ()
                  }
                ) |> ignore
              }
            )
          />
        </IconMenu>
      };
      let renderRow (article: Model.article) =>
        <tr key=article.id>
          <td> (renderMenuButton article) </td>
          <td>
            <a href=("#/article/" ^ article.id)> (se article.title) </a>
            (
              article.default ?
                <div className="floatRight" title="This article is readonly.">
                  <LockIcon className="icon" />
                </div> :
                ReasonReact.nullElement
            )
          </td>
        </tr>;
      <div>
        <MuiThemeProvider>
          <div>
            <ArticleDialog
              key="addArticleDialog"
              dialogTitle="Add Article"
              onOk=(self.update addArticle)
              onCancel=(self.update (toggleDialog ARCTICLE_DIALOG_CLOSED))
              isOpen=(self.state.openedDialog == ADD)
            />
            <ArticleDialog
              dialogTitle="Rename Article"
              onOk=(self.update renameArticle)
              onCancel=(self.update (toggleDialog ARCTICLE_DIALOG_CLOSED))
              initialArticleTitle=(
                switch self.state.openedDialog {
                | RENAME {title} => title
                | _ => ""
                }
              )
              isOpen=(
                switch self.state.openedDialog {
                | RENAME _ => true
                | _ => false
                }
              )
            />
            <RaisedButton label="Add Article" onClick=(self.update (toggleDialog ADD)) />
            <h1> (se "Articles") </h1>
            <table>
              <colgroup> <col className="menuColumn" /> </colgroup>
              <thead> <tr> <th /> <th> (se "Title") </th> </tr> </thead>
              <tbody>
                (
                  switch self.state.articles {
                  | Some repos => ReasonReact.arrayToElement (Array.map renderRow repos)
                  | None => <tr> <td /> <td> (se "Loading") </td> </tr>
                  }
                )
              </tbody>
            </table>
          </div>
        </MuiThemeProvider>
      </div>
    }
  }
};