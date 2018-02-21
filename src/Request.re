
/* module RequestHeaders = {
  type t;

  let make = () => ();

  let append = () => ();
  let delete = () => ();
  let entries = () => ();
  let get = () => ();
  let has = () => ();
  let keys = () => ();
  let set = () => ();
  let values = () => ();
}; */

let make = (
  ~meth: string,
  /* ~headers: option(RequestHeaders.t)=?, */
  url
) => {
  Js.Promise.make((~resolve, ~reject as _) => {
    let onReadyStateChange = (xhr, _evt) => {
      let readyState = XHR.getReadyState(xhr);
      switch readyState {
      | Unsent => Js.log("Unsent")
      | Opened => Js.log("Opened")
      | HeadersReceived => Js.log("Headers Received")
      | Loading => Js.log("Loading")
      | Done => Js.log("Done")
      };
    };

    let onLoad = (xhr, _evt) => {
      let response = XHR.getResponse(xhr);
      let result = switch response {
      | Empty => Js.Result.Error("Invalid response")
      | Text(_) | ArrayBuffer(_) | Blob(_) | Document(_) | Json(_) | Raw(_) => Js.Result.Ok(response)
      };
      [@bs] resolve(result);
    };

    let onAbort = (_xhr, _evt) => {
      Js.log("on abort");
    };

    let onError = (_xhr, _evt) => {
      Js.log("on error");
    };

    let onLoadStart = (_xhr, _evt) => {
      Js.log("on load start");
    };

    let onLoadEnd = (_xhr, _evt) => {
      Js.log("on load end");
    };

    let onProgress = (_xhr, _evt) => {
      Js.log("on progress");
    };

    let onTimeout = (_xhr, _evt) => {
      Js.log("on timeout");
    };

    XHR.create(meth, url)
      |> XHR.addListener(ReadyStateChange, onReadyStateChange)
      |> XHR.addListener(Load, onLoad)
      |> XHR.addListener(Abort, onAbort)
      |> XHR.addListener(Error, onError)
      |> XHR.addListener(LoadStart, onLoadStart)
      |> XHR.addListener(LoadEnd, onLoadEnd)
      |> XHR.addListener(Progress, onProgress)
      |> XHR.addListener(Timeout, onTimeout)
      |> XHR.send();

    /* XhrRe.setResponseType(Text, xhr); */
    /*
    let responseType = XhrRe.getResponseType(xhr);
    switch responseType {
    | Text => Js.log("Text")
    | Blob => Js.log("Blob")
    | ArrayBuffer => Js.log("ArrayBuffer")
    | Document => Js.log("Document")
    | Json => Js.log("Json")
    | Raw(raw) => Js.log(raw)
    }; */
  });
};

let get = (url) => {
  let meth = "GET";

  make(~meth, url);
};

let post = (url) => {
  let meth = "POST";

  make(~meth, url);
};

let put = (url) => {
  let meth = "PUT";

  make(~meth, url);
};

let patch = (url) => {
  let meth = "PATCH";

  make(~meth, url);
};

let delete = (url) => {
  let meth = "DELETE";

  make(~meth, url);
};

let head = (url) => {
  let meth = "HEAD";

  make(~meth, url);
};

let options = (url) => {
  let meth = "OPTIONS";

  make(~meth, url);
};
