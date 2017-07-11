open Bs_fetch;

type article = {
  id: string,
  title: string
};

let parseArticleJson json :article =>
  Json.Decode.{id: field "id" string json, title: field "title" string json};

let parseArticlesResponseJson json => (Json.Decode.array parseArticleJson) json;

let serviceBaseUrl = "http://localhost:8888/articles";

let fetchArticles () =>
  Bs_fetch.fetch serviceBaseUrl |> Js.Promise.then_ Bs_fetch.Response.text |>
  Js.Promise.then_ (
    fun jsonText =>
      Js.Promise.resolve (parseArticlesResponseJson (Js.Json.parseExn jsonText))
  );

let delete articleId =>
  Bs_fetch.fetchWithInit (serviceBaseUrl ^ "/" ^ articleId) (RequestInit.make method_::Delete ()) |>
  Js.Promise.then_ Bs_fetch.Response.text;


let create title => {
  let headers = Bs_fetch.HeadersInit.make {"Content-Type": "application/json"};
  let bodyJsObject = [%bs.obj {title: title } ];
  let body = bodyJsObject |> Js.Json.stringifyAny |> Option.get |> BodyInit.make;
  Bs_fetch.fetchWithInit serviceBaseUrl (RequestInit.make method_::Post ::headers ::body ()) |>
  Js.Promise.then_ Bs_fetch.Response.text
};