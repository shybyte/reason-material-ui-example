let se = ReasonReact.stringToElement;

open MaterialUi;

type dialogState = {
  articleTitle: string,
};

type dialogRetainedProps = {isOpen: bool};

let component = ReasonReact.statefulComponentWithRetainedProps "AddArticleDialog";

let make ::dialogTitle ::onOk ::onCancel ::initialArticleTitle="" ::isOpen _children => {
  let onChange newValue {ReasonReact.state: _state} =>
    ReasonReact.Update {articleTitle: newValue};
  {
    ...component,
    initialState: fun () => {articleTitle: initialArticleTitle},
    retainedProps: {isOpen: isOpen},
    willReceiveProps: fun oldSelf =>
      if (isOpen && not oldSelf.retainedProps.isOpen) {
        {articleTitle: initialArticleTitle}
      } else {
        oldSelf.state
      },
    render: fun self => {
      let okButton = <FlatButton label="OK" onClick=(fun () => onOk self.state.articleTitle) />;
      let cancelButton = <FlatButton label="Cancel" onClick=onCancel />;
      let actions = [|cancelButton, okButton|];
      <Dialog onRequestClose=onCancel isOpen title=dialogTitle actions>
        <form
          onSubmit=(
            fun ev => {
              ReactEventRe.Form.preventDefault ev;
              onOk self.state.articleTitle
            }
          )>
          <TextField
            ref=(fun el => {
              if (not (Js.Null.test el)) {
                Js.Global.setTimeout (fun () => {
                  (Obj.magic el)##input##focus();
                }) 200 |> ignore;
              }
            })
            onChange=(self.update onChange)
            autoFocus=true
            hintText="Article Title"
            value=self.state.articleTitle
            style=(ReactDOMRe.Style.make width::"100%" ())
          />
        </form>
      </Dialog>
    }
  }
};