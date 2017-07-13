let se = ReasonReact.stringToElement;

open MaterialUi;

type appState = {
  isDialogOpen: bool,
  articles: option (array Webservice.article)
};

let handleArticlesLoaded articles {ReasonReact.state: state} =>
  ReasonReact.Update {...state, articles: Some articles};

let component = ReasonReact.statefulComponent "ArticlesOverview";

let make _children => {
  let toggleDialog _event {ReasonReact.state: state} =>
    ReasonReact.Update {...state, isDialogOpen: not state.isDialogOpen};
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
    ReasonReact.Update {...self.state, isDialogOpen: false}
  };
  {
    ...component,
    initialState: fun () => {isDialogOpen: false, articles: None},
    didMount: fun self => reload self,
    render: fun self => {
      let toggleDialog = self.update toggleDialog;
      let renderMenuButton (article: Webservice.article) => {
        let iconButton = <IconButton> <MoreVertIcon /> </IconButton>;
        <IconMenu iconButtonElement=iconButton>
          <MenuItem primaryText="Edit" onTouchTap=(fun () => Js.log ("Edit " ^ article.title)) />
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
            onTouchTap=(fun () => Js.log ("Rename " ^ article.title))
          />
          <MenuItem
            primaryText="Delete"
            disabled=article.default
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
      let renderRow (article: Webservice.article) =>
        <tr key=article.id>
          <td> (renderMenuButton article) </td>
          <td> (se article.title) </td>
        </tr>;
      <div>
        <MuiThemeProvider>
          <div>
            <AddArticleDialog
              onOk=(self.update addArticle)
              onCancel=toggleDialog
              isOpen=self.state.isDialogOpen
            />
            <RaisedButton label="Add Article" onClick=toggleDialog />
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