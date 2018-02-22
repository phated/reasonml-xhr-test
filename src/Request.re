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

      let onLoad = (xhr, _evt) => {
        /* TODO: Should HEAD & OPTIONS resolve to something other than an empty body? */
        let xhrResponse = XHR.getResponse(xhr);
        let response = Response.fromXHR(xhrResponse);
        [@bs] resolve(Js.Result.Ok(response));
      };

      let onAbort = (_xhr, _evt) => [@bs] resolve(Js.Result.Error(Error.Abort));
      let onError = (_xhr, _evt) => [@bs] resolve(Js.Result.Error(Error.Network));
      let onTimeout = (_xhr, _evt) => [@bs] resolve(Js.Result.Error(Error.Timeout));

      xhr
        |> XHR.setTimeout(timeout)
        |> XHR.setResponseType(responseType)
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

let post = (~timeout=?, ~responseType=?, ~signal=?, url) => {
  let meth = XHR.Methods.POST;

  make(~meth, ~timeout?, ~responseType?, ~signal?, url);
};

let put = (~timeout=?, ~responseType=?, ~signal=?, url) => {
  let meth = XHR.Methods.PUT;

  make(~meth, ~timeout?, ~responseType?, ~signal?, url);
};

let patch = (~timeout=?, ~responseType=?, ~signal=?, url) => {
  let meth = XHR.Methods.PATCH;

  make(~meth, ~timeout?, ~responseType?, ~signal?, url);
};

let delete = (~timeout=?, ~responseType=?, ~signal=?, url) => {
  let meth = XHR.Methods.DELETE;

  make(~meth, ~timeout?, ~responseType?, ~signal?, url);
};

let head = (~timeout=?, ~responseType=?, ~signal=?, url) => {
  let meth = XHR.Methods.HEAD;

  make(~meth, ~timeout?, ~responseType?, ~signal?, url);
};

let options = (~timeout=?, ~responseType=?, ~signal=?, url) => {
  let meth = XHR.Methods.OPTIONS;

  make(~meth, ~timeout?, ~responseType?, ~signal?, url);
};
