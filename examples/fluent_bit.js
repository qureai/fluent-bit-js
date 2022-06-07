const fluent_bit = require("../index");

fluent_bit.configure({
  service: {
    flush: 1,
  },
  pipeline: {
    inputs: [
      {
        name: "cpu",
        config: {
          tag: "my_cpu",
        },
      },
    ],
    outputs: [
      {
        name: "stdout",
        config: {
          match: "*",
        },
      },
    ],
  },
});
fluent_bit.log("reconfigured");

setTimeout(() => console.log("bye"), 10000);
