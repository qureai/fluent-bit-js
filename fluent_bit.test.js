const { FluentBit } = require("./fluent_bit");

test("import successful", () => {
  true;
});

test("create context", () => {
  const service = new FluentBit();
  expect(service).not.toBeNull();
});

test("create input", () => {
  const service = new FluentBit();
  const input = service.input("lib");
  console.log('input number', input)
  expect(typeof input).toBe('number')
});

// test("set input config", () => {
//   const service = new fluentbit();
//   const input = service.input("cpu");
//   input.set({ tag: "my_records", ssl: false });
// });

test("create output", () => {
  const service = new FluentBit();
  const output = service.output("stdout");
  console.log('output number', output)
  expect(typeof output).toBe('number');
});

// test("set output config", () => {
//   const service = new fluentbit();
//   const output = service.output("stdout");
//   output.set({ tag: "my_records", ssl: false });
// });

test("service start", () => {
  const service = new FluentBit();
  const input = service.input("cpu");
  const output = service.output("stdout");
  service.start();
  console.log("service started fine");
});

test("log output", () => {
  const service = new FluentBit();
  const input = service.input("lib");
  const output = service.output("stdout");
  service.start();
  const data = '[1449505010, {"key1": "some value"}]';
  const response = service.lib_push(input, data);
  expect(response).not.toBe(-1);
  setTimeout(() => {
    console.log('blah blah');
  }, 10000)
});