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

let abortable = Abortable.make();

let result = Request.get(~signal=abortable.signal, "https://swapi.co/api/people/1/");
/* let result = Request.get(~signal=abortable.signal, "http://localhost:8080/api/organization/undertakerjs/1065786"); */

/* abortable.abort(); */

let parse = (content) => {
  switch (Json.parse(content)) {
  | Some(json) => Js.log(Person.decode(json))
  | None => Js.log("parse failed")
  }
};

Js.Promise.(
  result
    |> then_((res) => {
      switch res {
      | Js.Result.Ok(Response.Text(content)) => parse(content)
      | Js.Result.Ok(response) => Js.log2("Unexpected Response", response)
      | Js.Result.Error(Request.Error.Timeout) => Js.log("request timed out")
      | Js.Result.Error(Request.Error.Abort) => Js.log("request aborted")
      | Js.Result.Error(Request.Error.Network) => Js.log("network error")
      };
      resolve();
    })
);
