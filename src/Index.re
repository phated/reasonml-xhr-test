
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

let result = Request.get("https://swapi.co/api/people/1/");


Js.Promise.(
  result
    |> then_((res) => {
      switch res {
      | Js.Result.Ok(response) => Js.log(response)
      | Js.Result.Error(err) => Js.log(err)
      };
      resolve();
    })
);
