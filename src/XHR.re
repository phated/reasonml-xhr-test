/* Raw values from JS */
type unresolved;

/* Types that are not implemented yet */
type arrayBuffer;
type blob;

/* Aliased types */
type document = Webapi.Dom.Document.t;

module ResponseType = {
  type t =
    | Text
    | ArrayBuffer
    | Blob
    | Document
    | Json
    | Raw(string);
};

module Response = {
  type t =
    | Text(string)
    | ArrayBuffer(arrayBuffer)
    | Blob(blob)
    | Document(document)
    | Json(Js.Json.t)
    | Empty
    | Raw(unresolved);
};

module Event = {
  type t =
    | ReadyStateChange
    | Abort
    | Error
    | Load
    | LoadStart
    | LoadEnd
    | Progress
    | Timeout;

  type readystatechange = Js.Json.t;
  type abort = Js.Json.t;
  type error = Js.Json.t;
  type load = Js.Json.t;
  type loadstart = Js.Json.t;
  type progress = Js.Json.t;
  type timeout = Js.Json.t;
  type loadend = Js.Json.t;
};

module XMLHttpRequest = {
  type t;

  [@bs.deriving {jsConverter:newType}]
  type readyState =
    | Unsent
    | Opened
    | HeadersReceived
    | Loading
    | Done;

  type eventHandler('a) = 'a => unit;

  [@bs.new] external make : unit => t = "XMLHttpRequest";

  [@bs.send] external abort : t => unit = "";
  [@bs.send] [@bs.return nullable] external getAllResponseHeaders : t => option(string) = "";
  [@bs.send] [@bs.return nullable] external getResponseHeader : t => option(string) = "";
  /* Purposely ignore async/user/password arguments */
  [@bs.send] external open_ : (t, string, string) => unit = "open";
  [@bs.send] external overrideMimeType : (t, string) => unit = "";
  /* TODO: Overload send for other types */
  [@bs.send] external send : t => unit = "";
  [@bs.send] external setRequestHeader : (t, string, string) => unit = "";

  /* onreadystatechange */
  [@bs.set] external setOnReadyStateChange : (t, eventHandler(Event.readystatechange)) => unit = "onreadystatechange";
  [@bs.get] [@bs.return nullable] external getOnReadyStateChange : t => option(eventHandler(Event.readystatechange)) = "onreadystatechange";

  /* onload */
  [@bs.set] external setOnLoad : (t, eventHandler(Event.load)) => unit = "onload";
  [@bs.get] external getOnLoad : t => eventHandler(Event.load) = "onload";

  /* responseType */
  [@bs.set] external setResponseType : (t, string) => unit = "responseType";
  [@bs.get] external getResponseType : t => string = "responseType";

  /* withCredentials */
  [@bs.set] external setWithCredentials : (t, Js.boolean) => unit = "withCredentials";
  [@bs.get] external getWithCredentials : (t) => unit = "withCredentials";

  /* Read-only but still naming with "get" to be explicit */
  [@bs.get] external getReadyState : t => readyState = "readyState";
  [@bs.get] [@bs.return nullable] external getResponse : t => option(unresolved) = "response";
  /* Custom methods to get response as workable values */
  [@bs.get] [@bs.return nullable] external getResponseAsText : t => option(string) = "response";
  [@bs.get] [@bs.return nullable] external getResponseAsJson : t => option(Js.Json.t) = "response";
  [@bs.get] [@bs.return nullable] external getResponseAsArrayBuffer : t => option(arrayBuffer) = "response";
  [@bs.get] [@bs.return nullable] external getResponseAsBlob : t => option(blob) = "response";
  [@bs.get] [@bs.return nullable] external getResponseAsDocument : t => option(document) = "response";

  /* TODO: this can throw InvalidStateError when responseType is not "text" so maybe drop this? */
  [@bs.get] external getResponseText : t => string = "responseText";
};

let make = () => {
  XMLHttpRequest.make();
};

let open_ = (method, url, xhr) => {
  XMLHttpRequest.open_(xhr, method, url);
  /* Pipeable */
  xhr;
};

/* Convenience method over make + open */
/* WARNING: Don't use this if you need to listen for readystatechange before open */
let create = (method, url) => {
  /* Pipeable */
  make() |> open_(method, url);
};

let addListener = (evt: Event.t, handler, xhr) => {
  /* Partial the xhr onto the handler so we can define the handlers before our xhr chain */
  let callback = handler(xhr);
  switch evt {
  | ReadyStateChange => XMLHttpRequest.setOnReadyStateChange(xhr, callback)
  | Load => XMLHttpRequest.setOnLoad(xhr, callback)
  };
  /* Pipeable */
  xhr;
};

/* TODO: Add support for sending body */
let send = ((), xhr) => {
  XMLHttpRequest.send(xhr);
  /* Not pipeable - should end pipe chain */
  ();
};

let getReadyState = (xhr) => {
  XMLHttpRequest.getReadyState(xhr);
};

let getResponseType = (xhr) => {
  let responseType = XMLHttpRequest.getResponseType(xhr);
  switch responseType {
  | "" | "text" => ResponseType.Text
  | "arraybuffer" => ResponseType.ArrayBuffer
  | "blob" => ResponseType.Blob
  | "document" => ResponseType.Document
  | "json" => ResponseType.Json
  | raw => ResponseType.Raw(raw)
  };
};

let setResponseType = (responseType, xhr) => {
  let setResponseType = XMLHttpRequest.setResponseType(xhr);
  switch responseType {
  | ResponseType.Text => setResponseType("text")
  | ResponseType.ArrayBuffer => setResponseType("arraybuffer")
  | ResponseType.Blob => setResponseType("blob")
  | ResponseType.Document => setResponseType("document")
  | ResponseType.Json => setResponseType("json")
  | ResponseType.Raw(raw) => setResponseType(raw)
  }
};

let getResponse = (xhr) => {
  let responseType = getResponseType(xhr);
  switch responseType {
  | ResponseType.Text => {
    let response = XMLHttpRequest.getResponseAsText(xhr);
    switch response {
    | Some(content) => Response.Text(content)
    | None => Response.Empty
    };
  }
  | ResponseType.ArrayBuffer => {
    let response = XMLHttpRequest.getResponseAsArrayBuffer(xhr);
    switch response {
    | Some(content) => Response.ArrayBuffer(content)
    | None => Response.Empty
    };
  }
  | ResponseType.Blob => {
    let response = XMLHttpRequest.getResponseAsBlob(xhr);
    switch response {
    | Some(content) => Response.Blob(content)
    | None => Response.Empty
    };
  }
  | ResponseType.Document => {
    let response = XMLHttpRequest.getResponseAsDocument(xhr);
    switch response {
    | Some(content) => Response.Document(content)
    | None => Response.Empty
    };
  }
  | ResponseType.Json => {
    let response = XMLHttpRequest.getResponseAsJson(xhr);
    switch response {
    | Some(content) => Response.Json(content)
    | None => Response.Empty
    };
  }
  | ResponseType.Raw(_) => {
    let response = XMLHttpRequest.getResponse(xhr);
    switch response {
    | Some(content) => Response.Raw(content)
    | None => Response.Empty
    };
  }
}
}
