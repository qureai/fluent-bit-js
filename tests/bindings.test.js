const { FluentBit: FluentBitRaw } = require("bindings")("fluent_bit_js.node");

test("import successful", () => {
  true;
});

test("create context", () => {
  const service = new FluentBitRaw();
  expect(service).not.toBeNull();
});

test("create input config", () => {
  const service = new FluentBitRaw();
  const input = service.input("cpu");
  expect(typeof input).toBe("number");
  response = service.input_set(input, "tag", "my_records");
  expect(response).not.toBe(-1);
});

test("create filter config", () => {
  const service = new FluentBitRaw();
  const filter = service.filter("parser");
  expect(typeof filter).toBe("number");
  response = service.filter_set(filter, "match", "*");
  expect(response).not.toBe(-1);
});

test("create output config", () => {
  const service = new FluentBitRaw();
  const input = service.input("cpu");
  expect(typeof input).toBe("number");
  response = service.input_set(input, "tag", "my_records");
  expect(response).not.toBe(-1);
});

test("service start", () => {
  const service = new FluentBitRaw();
  const input = service.input("cpu");
  const output = service.output("stdout");
  service.start();
});

test("log output", () => {
  const service = new FluentBitRaw();
  const input = service.input("lib");
  const output = service.output("stdout");
  service.start();
  const data = '[1449505010, {"key1": "some value"}]';
  const response = service.lib_push(input, data);
  expect(response).not.toBe(-1);
});

test("log mem filter", () => {
  // https://docs.fluentbit.io/manual/v/1.8/pipeline/filters/modify#configuration-file
  const service = new FluentBitRaw();
  const input = service.input("mem");
  service.input_set(input, "tag", "mem.local");

  const output = service.output("stdout");
  service.output_set(output, "match", "*");

  const filter = service.filter("modify");
  service.filter_set(
    filter,
    "match",
    "*",
    "add",
    "Service1 SOMEVALUE",
    "Rename",
    "Mem.used MEMUSED"
  );
  service.start();
});
