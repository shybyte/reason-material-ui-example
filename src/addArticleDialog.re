let se = ReasonReact.stringToElement;

open MaterialUi;

type dialogState = {title: string};

let component = ReasonReact.statefulComponentWithRetainedProps "AddArticleDialog";

let make ::onOk ::onCancel isOpen::(isOpen: bool) _children => {
  let onChange newValue {ReasonReact.state: _state} => ReasonReact.Update {title: newValue};
  {
    ...component,
    initialState: fun () => {title: ""},
    retainedProps: isOpen,
    didMount: fun _self => ReasonReact.NoUpdate,
    willReceiveProps: fun self => {
      if (self.retainedProps && not isOpen) {{title: ""}} else {self.state}},
    render: fun self => {
      let okButton = <FlatButton label="OK" onClick=(fun () => onOk self.state.title) />;
      let cancelButton = <FlatButton label="Cancel" onClick=onCancel />;
      let actions = [|cancelButton, okButton|];
      <Dialog onRequestClose=onCancel isOpen title="Add Article" actions>
        <form
          onSubmit=(
            fun ev => {
              ReactEventRe.Form.preventDefault ev;
              onOk self.state.title
            }
          )>
          <TextField
            onChange=(self.update onChange)
            autoFocus=true
            hintText="Article Title"
            value=self.state.title
            style=(ReactDOMRe.Style.make  width::"100%" ())
          />
        </form>
      </Dialog>
    }
  }
};