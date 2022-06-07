const { FluentBit: FluentBitRaw } = require("bindings")("fluent_bit_js.node");

class FluentBit {
  constructor() {
    console.log("hi");
  }
}

module.exports = { FluentBit };
