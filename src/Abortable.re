module Signal = {

  type aborted = ref(bool);
  type listener = unit => unit;
  type listen = listener => unit;

  type t = {
    aborted: aborted,
    listen: listen,
    dispatch: unit => unit
  };

  let make = () => {
    let aborted = ref(false);
    let listeners = ref([]);

    let listen = (listener: listener) => {
      listeners := List.append([listener], listeners^);
    };

    let dispatch = () => {
      if (!aborted^) {
        aborted := true;
        List.iter(listener => [@bs.uncurry] listener(), listeners^);
      };
    };

    let signal = {
      aborted: aborted,
      listen: listen,
      dispatch: dispatch,
    };

    signal;
  };
};

type signal = {
  aborted: Signal.aborted,
  listen: Signal.listen,
};

type t = {
  signal: signal,
  abort: unit => unit
};

let make = () => {
  let internalSignal = Signal.make();

  let signal = {
    aborted: internalSignal.aborted,
    listen: internalSignal.listen,
  };

  let abort = () => {
    internalSignal.dispatch();
  };

  let controller = {
    signal: signal,
    abort: abort
  };

  controller;
};
