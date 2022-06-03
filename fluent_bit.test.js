const fluentbit = require("./fluent_bit");

test("import successful", () => {
  true;
});

test("create context", () => {
  const service = new fluentbit();
  expect(service).not.toBeNull();
});

test("create input", () => {
  service = new fluentbit();
  const input = service.input("cpu");
  expect(input).not.toBeNull();
});

test("set input config", () => {
  const service = new fluentbit();
  const input = service.input("cpu");
  input.set({ tag: "my_records", ssl: false });
});

test("create output", () => {
  const service = new fluentbit();
  const output = service.output("stdout");
  expect(output).not.toBeNull();
});

test("set output config", () => {
  const service = new fluentbit();
  const output = service.output("stdout");
  output.set({ tag: "my_records", ssl: false });
});

test("service start", () => {
  const service = new fluentbit();
  service.set({ Flush: 1 });
  const input = service.input("cpu");
  input.set({ tag: "my_records", ssl: false });
  const output = service.output("stdout");
  output.set({ tag: "my_records", ssl: false });
  service.start();
});

test("log output", () => {
  const service = new fluentbit();
  service.set({ Flush: 1 });
  const input = service.input("cpu");
  input.set({ tag: "my_records", ssl: false });
  const output = service.output("stdout");
  output.set({ tag: "my_records", ssl: false });
  service.start();
  input.log("happy logging with fluent-bit");
});

test("service stop", () => {
  const service = new fluentbit();
  service.set({ Flush: 1 });
  const input = service.input("cpu");
  input.set({ tag: "my_records", ssl: false });
  const output = service.output("stdout");
  output.set({ tag: "my_records", ssl: false });
  service.start();
  input.log("happy logging with fluent-bit");

  service.stop();
});

test("service destroy", () => {
  const service = new fluentbit();
  service.set({ Flush: 1 });
  const input = service.input("cpu");
  input.set({ tag: "my_records", ssl: false });
  const output = service.output("stdout");
  output.set({ tag: "my_records", ssl: false });
  service.start();
  input.log("happy logging with fluent-bit");

  service.stop();
  service.destroy();
});
