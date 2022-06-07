const { FluentBit: FluentBitRaw } = require("bindings")("fluent_bit_js.node");

class FluentBit {
  constructor(config) {
    this.service = new FluentBitRaw()
  }
}

module.exports = { FluentBit };
