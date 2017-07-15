open Bs_fetch;

open Model;

let parseArticleJson json :article =>
  Json.Decode.{id: field "id" string json, title: field "title" string json, default: field "default" bool json};

let parseArticlesResponseJson json => (Json.Decode.array parseArticleJson) json;

let serviceBaseUrl = "/articles";

let fetchArticles () =>
  Bs_fetch.fetch serviceBaseUrl |> Js.Promise.then_ Bs_fetch.Response.text |>
  Js.Promise.then_ (
    fun jsonText => Js.Promise.resolve (parseArticlesResponseJson (Js.Json.parseExn jsonText))
  );

let fetchArticle id =>
  Bs_fetch.fetch (serviceBaseUrl ^ "/" ^ id) |> Js.Promise.then_ Bs_fetch.Response.text |>
  Js.Promise.then_ (
    fun jsonText => Js.Promise.resolve (parseArticleJson (Js.Json.parseExn jsonText))
  );  

let delete articleId =>
  Bs_fetch.fetchWithInit (serviceBaseUrl ^ "/" ^ articleId) (RequestInit.make method_::Delete ()) |>
  Js.Promise.then_ Bs_fetch.Response.text;

let create title => {
  let headers = Bs_fetch.HeadersInit.make {"Content-Type": "application/json"};
  let bodyJsObject = {"title": title, "default": Js.Boolean.to_js_boolean false};
  let body = bodyJsObject |> Js.Json.stringifyAny |> Option.get |> BodyInit.make;
  Bs_fetch.fetchWithInit serviceBaseUrl (RequestInit.make method_::Post ::headers ::body ()) |>
  Js.Promise.then_ Bs_fetch.Response.text
};

let rename articleId newTitle => {
  let headers = Bs_fetch.HeadersInit.make {"Content-Type": "application/json"};
  let bodyJsObject = {"title": newTitle};
  let body = bodyJsObject |> Js.Json.stringifyAny |> Option.get |> BodyInit.make;
  Bs_fetch.fetchWithInit (serviceBaseUrl ^ "/" ^ articleId) (RequestInit.make method_::Patch ::headers ::body ()) |>
  Js.Promise.then_ Bs_fetch.Response.text
};