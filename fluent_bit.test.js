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
  expect(typeof input).toBe("number");
});

test("set input config", () => {
  const service = new FluentBit();
  const input = service.input("cpu");
  expect(typeof input).toBe("number");
  response = service.input_set(input, "tag", "my_records");
  expect(response).not.toBe(-1);
});

test("create output", () => {
  const service = new FluentBit();
  const output = service.output("stdout");
  expect(typeof output).toBe("number");
});

test("set output config", () => {
  const service = new FluentBit();
  const input = service.input("cpu");
  expect(typeof input).toBe("number");
  response = service.input_set(input, "tag", "my_records");
  expect(response).not.toBe(-1);
});

test("service start", () => {
  const service = new FluentBit();
  const input = service.input("cpu");
  const output = service.output("stdout");
  service.start();
});

test("log output", () => {
  const service = new FluentBit();
  const input = service.input("lib");
  const output = service.output("stdout");
  service.start();
  const data = '[1449505010, {"key1": "some value"}]';
  const response = service.lib_push(input, data);
  expect(response).not.toBe(-1);
});
