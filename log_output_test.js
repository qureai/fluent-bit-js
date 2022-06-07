const { FluentBit } = require("./fluent_bit");

const service = new FluentBit();
const input = service.input("mem");
service.input_set(input, "tag", "mem.local");

const lib_input = service.input("lib");
service.input_set(lib_input, "tag", "console");

const filter = service.filter("modify");
service.filter_set(
  filter,
  "match",
  "mem.local",
  "add",
  "Service1 SOMEVALUE",
  "Rename",
  "Mem.used MEMUSED"
);

const output = service.output("stdout");
service.output_set(output, "match", "*");

service.start();

setInterval(() => {
  const data = [Date.now(), { key1: "some value" }];
  const d = JSON.stringify(data);
  service.lib_push(lib_input, d);
}, 1000);
