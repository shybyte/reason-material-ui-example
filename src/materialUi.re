external raisedButton: ReasonReact.reactClass = "default" [@@bs.val] [@@bs.module "material-ui/RaisedButton"];
external muiThemeProvider: ReasonReact.reactClass = "default" [@@bs.val] [@@bs.module "material-ui/styles/MuiThemeProvider"];
external flatButton: ReasonReact.reactClass = "default" [@@bs.val] [@@bs.module "material-ui/FlatButton"];
external dialog: ReasonReact.reactClass = "default" [@@bs.val] [@@bs.module "material-ui/Dialog"];
external textfield: ReasonReact.reactClass = "default" [@@bs.val] [@@bs.module "material-ui/TextField"];
external injectTapEventPlugin : unit =>  unit = "react-tap-event-plugin" [@@bs.module];

external iconMenu: ReasonReact.reactClass = "default" [@@bs.val] [@@bs.module "material-ui/IconMenu"];
external menuItem: ReasonReact.reactClass = "default" [@@bs.val] [@@bs.module "material-ui/MenuItem"];
external iconButton: ReasonReact.reactClass = "default" [@@bs.val] [@@bs.module "material-ui/IconButton"];
external moreVertIcon: ReasonReact.reactClass = "default" [@@bs.val] [@@bs.module "material-ui/svg-icons/navigation/more-vert"];
external lock: ReasonReact.reactClass = "default" [@@bs.val] [@@bs.module "material-ui/svg-icons/action/lock"];

let component = ReasonReact.statelessComponent "Component";

module RaisedButton = {
  let make ::label ::onClick children =>
    ReasonReact.wrapJsForReason
      reactClass::raisedButton
      props::{
        "label": label, /* OCaml string maps to JS string, no conversion needed here */
        "onClick": onClick /* OCaml string maps to JS string, no conversion needed here */
      }
      children;
};

module FlatButton = {
  let make ::label ::onClick children =>
    ReasonReact.wrapJsForReason
      reactClass::flatButton
      props::{
        "label": label, /* OCaml string maps to JS string, no conversion needed here */
        "onClick": onClick /* OCaml string maps to JS string, no conversion needed here */
      }
      children;
};

module MuiThemeProvider = {
  let make children =>
    ReasonReact.wrapJsForReason
      reactClass::muiThemeProvider
      props::{
        "className": "className" 
      }
      children;
};

module Dialog = {
  let make ::onRequestClose ::isOpen ::title=? ::actions children =>
    ReasonReact.wrapJsForReason
      reactClass::dialog
      props::{
        "title": Js.Null_undefined.from_opt title,
        "onRequestClose": onRequestClose, 
        "open": Js.Boolean.to_js_boolean isOpen, 
        "actions": actions 
      }
      children;
};

module IconMenu = {
  let make ::className=? ::iconButtonElement children =>
    ReasonReact.wrapJsForReason
      reactClass::iconMenu
      props:: {
        "className": Js.Null_undefined.from_opt className,
        "iconButtonElement": iconButtonElement
      }
      children;
};

module IconButton = {
  let make ::className=? children =>
    ReasonReact.wrapJsForReason
      reactClass::iconButton
      props:: {
        "className": Js.Null_undefined.from_opt className
      }
      children;
};

module MoreVertIcon = {
  let make ::className=? children =>
    ReasonReact.wrapJsForReason
      reactClass::moreVertIcon
      props:: {
        "className": Js.Null_undefined.from_opt className
      }
      children;
};

module LockIcon = {
  let make ::className=? ::title=? children =>
    ReasonReact.wrapJsForReason
      reactClass::lock
      props:: {
        "className": Js.Null_undefined.from_opt className,
        "title": Js.Null_undefined.from_opt title,
        "style": {
          "width": "16px",
          "height": "16px"
        }
      }
      children;
};

module MenuItem = {
  let make ::primaryText ::onTouchTap ::title=? ::rightIcon=? ::className=? ::disabled=? children =>
    ReasonReact.wrapJsForReason
      reactClass::menuItem
      props:: {
        "className": Js.Null_undefined.from_opt className,
        "disabled": disabled |> Option.default false |> Js.Boolean.to_js_boolean  ,
        "primaryText": primaryText,
        "onTouchTap": onTouchTap,
        "rightIcon": Js.Null_undefined.from_opt rightIcon,
        "title": Js.Null_undefined.from_opt title
      }
      children;
};

module TextField = {
  let make ::id=? ::value ::hintText=? ::autoFocus=? ::onChange style::(style: option ReactDOMRe.style)=? children =>
    ReasonReact.wrapJsForReason
      reactClass::textfield
      props:: {
        "id": id |> Option.default "textfieldId" ,
        "autoFocus": autoFocus |> Option.default false |> Js.Boolean.to_js_boolean  ,
        "value": value,
        "hintText": hintText |> Option.default "",
        "onChange": fun _ev value => {
          onChange value;
        },
        "style": style |> Option.default (ReactDOMRe.Style.make ())
      }
      children;
};