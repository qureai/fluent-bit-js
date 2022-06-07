const { FluentBit } = require("./fluent_bit");
const service = new FluentBit();
const input = service.input("lib");
const output = service.output("stdout");
service.start();
const data = "[1449505010, {\"key1\": \"some value\"}]";
console.log("data", data);
const response = service.lib_push(input, data, data.length);
setTimeout(() => {
  console.log('blah blah');
}, 10000)