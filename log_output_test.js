const { FluentBit } = require("./fluent_bit");
const service = new FluentBit();

const cpu_input = service.input("cpu");
service.input_set(cpu_input, "tag", "my_cpu", "Interval_Sec", "5");

const input = service.input("lib");
service.input_set(input, "tag", "lol");

const output = service.output("stdout");
service.output_set("output", "match", "*");

service.start();

const data = [1449505010, { key1: "some value" }];
const d = JSON.stringify(data);
const response = service.lib_push(input, d);

setTimeout(() => {
  console.log("blah blah");
}, 100000);
