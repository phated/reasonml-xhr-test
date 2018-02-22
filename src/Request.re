
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
  ~meth,
  /* ~headers: option(RequestHeaders.t)=?, */
  ~timeout=0,
  /* TODO: This is for parsing (I think), does it belong? */
  ~responseType=XHR.ResponseType.Text,
  ~signal: option(Abortable.signal)=?,
  url
) => {
  let aborted = ref(false);

  Js.Promise.make((~resolve, ~reject as _) => {
    aborted := switch signal {
    | Some(signal) => signal.aborted^
    | None => false
    };

    if (aborted^) {
      [@bs] resolve(Js.Result.Error(Error.Abort));
    } else {
      let xhr = XHR.create(meth, url);

      /* TODO: Should this be used for anything? */
      let onReadyStateChange = (_xhr, _evt) => {
        /* let readyState = XHR.getReadyState(xhr);
        switch readyState {
        | Unsent => Js.log("Unsent")
        | Opened => Js.log("Opened")
        | HeadersReceived => Js.log("Headers Received")
        | Loading => Js.log("Loading")
        | Done => Js.log("Done")
        }; */
        ();
      };

      let onLoad = (xhr, _evt) => {
        /* TODO: Should HEAD & OPTIONS resolve to something other than an empty body? */
        let response = XHR.getResponse(xhr);
        [@bs] resolve(Js.Result.Ok(response));
      };

      let onAbort = (_xhr, _evt) => [@bs] resolve(Js.Result.Error(Error.Abort));
      let onError = (_xhr, _evt) => [@bs] resolve(Js.Result.Error(Error.Network));
      let onTimeout = (_xhr, _evt) => [@bs] resolve(Js.Result.Error(Error.Timeout));

      xhr
        |> XHR.setTimeout(timeout)
        |> XHR.setResponseType(responseType)
        |> XHR.setListener(ReadyStateChange, onReadyStateChange)
        |> XHR.setListener(Load, onLoad)
        |> XHR.setListener(Abort, onAbort)
        |> XHR.setListener(Error, onError)
        |> XHR.setListener(Timeout, onTimeout)
        |> XHR.send();

      switch signal {
      | Some(signal) => signal.listen(() => XHR.abort(xhr))
      | None => ()
      };
    };
  });
};

let get = (~timeout=?, ~responseType=?, ~signal=?, url) => {
  let meth = XHR.Methods.GET;

  make(~meth, ~timeout?, ~responseType?, ~signal?, url);
};

let post = (~timeout=?, url) => {
  let meth = XHR.Methods.POST;

  make(~meth, ~timeout?, url);
};

let put = (~timeout=?, url) => {
  let meth = XHR.Methods.PUT;

  make(~meth, ~timeout=?, url);
};

let patch = (~timeout=?, url) => {
  let meth = XHR.Methods.PATCH;

  make(~meth, ~timeout?, url);
};

let delete = (~timeout=?, url) => {
  let meth = XHR.Methods.DELETE;

  make(~meth, ~timeout?, url);
};

let head = (~timeout=?, url) => {
  let meth = XHR.Methods.HEAD;

  make(~meth, ~timeout?, url);
};

let options = (~timeout=?, url) => {
  let meth = XHR.Methods.OPTIONS;

  make(~meth, ~timeout?, url);
};
