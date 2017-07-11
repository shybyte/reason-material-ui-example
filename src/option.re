/* https://github.com/ygrek/ocaml-extlib */
exception No_value;

let get = fun
  | Some v => v
  | None => raise No_value;

let default v => fun
  | Some v => v
  | None => v;

let map f => fun
  | Some v => Some (f v)
  | None => None;

let flatMap f => fun
  | Some v => f v
  | None => None;

let isSome = fun
  | None => false
  | _ => true;

let isNone = fun
  | None => true
  | _ => false;
