external raisedButton: ReasonReact.reactClass = "default" [@@bs.val] [@@bs.module "material-ui/RaisedButton"];
external muiThemeProvider: ReasonReact.reactClass = "default" [@@bs.val] [@@bs.module "material-ui/styles/MuiThemeProvider"];
external flatButton: ReasonReact.reactClass = "default" [@@bs.val] [@@bs.module "material-ui/FlatButton"];
external dialog: ReasonReact.reactClass = "default" [@@bs.val] [@@bs.module "material-ui/Dialog"];
external injectTapEventPlugin : unit =>  unit = "react-tap-event-plugin" [@@bs.module];

external iconMenu: ReasonReact.reactClass = "default" [@@bs.val] [@@bs.module "material-ui/IconMenu"];
external menuItem: ReasonReact.reactClass = "default" [@@bs.val] [@@bs.module "material-ui/MenuItem"];
external iconButton: ReasonReact.reactClass = "default" [@@bs.val] [@@bs.module "material-ui/IconButton"];
external moreVertIcon: ReasonReact.reactClass = "default" [@@bs.val] [@@bs.module "material-ui/svg-icons/navigation/more-vert"];

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

module MenuItem = {
  let make ::primaryText ::onTouchTap ::className=? children =>
    ReasonReact.wrapJsForReason
      reactClass::menuItem
      props:: {
        "className": Js.Null_undefined.from_opt className,
        "primaryText": primaryText,
        "onTouchTap": onTouchTap
      }
      children;
};