
/* let req = Xhr.create();

Xhr.open_("get", "https://swapi.co/api/people/1", req);

Xhr.send(EmptyBody, req);

module Person = {
  type t = {
    name: string,
    gender: string,
  };

  let decode = (json) => {
    Json.Decode.{
      name: json |> field("name", string),
      gender: json |> field("gender", string),
    };
  };
};

type result = Some(Person.t) | None;

Xhr.set_onload((_event) => {
  let respType = Xhr.get_responseType(req);

  let body = switch respType {
  | StringResponseType => Xhr.get_responseText(req)
  | _ => ""
  };

  let result: result = switch (Json.parse(body)) {
  | Some(json) => Some(Person.decode(json))
  | None => None
  };

  switch result {
  | Some(person) => Js.log(person)
  | None => Js.log("something went wrong")
  }
}, req); */

/* let reReq = Request.make(); */

let abortable = Abortable.make();

/* let result = Request.get(~signal=abortable.signal, "https://swapi.co/api/people/1/"); */
let result = Request.get(~signal=abortable.signal, "http://localhost:8080/api/organization/undertakerjs/1065786");

/* Js.Global.setTimeout(() => abortable.abort(), 1000); */
abortable.abort();

Js.Promise.(
  result
    |> then_((res) => {
      switch res {
      | Js.Result.Ok(response) => Js.log(response)
      | Js.Result.Error(Request.Error.Timeout) => Js.log("request timed out")
      | Js.Result.Error(Request.Error.Abort) => Js.log("request aborted")
      | Js.Result.Error(Request.Error.Network) => Js.log("network error")
      };
      resolve();
    })
);
