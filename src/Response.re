include XHR.Response;

let fromXHR = (xhrResponse: XHR.Response.t) => {
  switch xhrResponse {
  | Text(content) => Text(content)
  | ArrayBuffer(content) => ArrayBuffer(content)
  | Blob(content) => Blob(content)
  | Document(content) => Document(content)
  | Json(content) => Json(content)
  | Empty => Empty
  | Raw(content) => Raw(content)
  };
};
