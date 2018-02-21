
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

module Error = {
  type t =
    | Network
    | Abort
    | Timeout;
};

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
      [@bs] resolve(Js.Result.Ok(response));
    };

    let onAbort = (_xhr, _evt) => {
      [@bs] resolve(Js.Result.Error(Error.Abort));
    };

    let onError = (_xhr, _evt) => {
      [@bs] resolve(Js.Result.Error(Error.Network))
    };

    let onTimeout = (_xhr, _evt) => {
      [@bs] resolve(Js.Result.Error(Error.Timeout));
    };

    /* Probably don't need these */
    /* let onLoadStart = (_xhr, _evt) => {
      Js.log("on load start");
    };

    let onLoadEnd = (_xhr, _evt) => {
      Js.log("on load end");
    };

    let onProgress = (_xhr, _evt) => {
      Js.log("on progress");
    }; */

    XHR.create(meth, url)
      /* |> XHR.setTimeout(1) */
      |> XHR.setListener(ReadyStateChange, onReadyStateChange)
      |> XHR.setListener(Load, onLoad)
      |> XHR.setListener(Abort, onAbort)
      |> XHR.setListener(Error, onError)
      /* |> XHR.setListener(LoadStart, onLoadStart) */
      /* |> XHR.setListener(LoadEnd, onLoadEnd) */
      /* |> XHR.setListener(Progress, onProgress) */
      |> XHR.setListener(Timeout, onTimeout)
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
