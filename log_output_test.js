const { FluentBit } = require("./fluent_bit");
const service = new FluentBit();
const data = [1449505010, {"key1": "some value"}];
const d = JSON.stringify(data);
const input = service.input("lib");
const output = service.output("stdout");
service.start();
const response = service.lib_push(input, d);
setTimeout(() => {
  console.log('blah blah');
}, 10000)