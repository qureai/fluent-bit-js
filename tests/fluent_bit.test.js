const fluent_bit = require("../index.js");

test("import successful", () => {
  true;
});

test("log", () => {
  fluent_bit.log("hi");
});

test("config", () => {
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
});



test("multiple config", () => {
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
  console.log("configuring again");
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
  
});
