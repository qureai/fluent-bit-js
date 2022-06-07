const { FluentBit: FluentBitRaw } = require("bindings")("fluent_bit_js.node");

function flb_flatten(obj) {
  const args = [];
  for (const [key, value] of Object.entries(obj)) {
    args.push(String(key));
    args.push(String(value));
  }
  return args;
}

class FluentBit {
  constructor() {
    // default config
    this.configure();
  }

  configure(config) {
    // config = works same as yaml, except in json format
    if (this.service) {
      this.service.destroy();
      delete this.service;
    }

    this.service = new FluentBitRaw();
    this.lib_input = this.service.input("lib");
    this.service.input_set(this.lib_input, "tag", "console");

    if (config) {
      // service
      if (config.service) {
        const ret = this.service.service_set(...flb_flatten(config.service));
        if (ret < 0) {
          throw Error(`error in service settings: ${config.service}`);
        }
      }

      if (config.pipeline) {
        // inputs
        if (config.pipeline.inputs) {
          config.pipeline.inputs.map(({ name, config }) => {
            const input = this.service.input(String(name));
            if (input < 0) {
              throw Error(`error in input plugin name : ${name}`);
            }
            const ret = this.service.input_set(input, ...flb_flatten(config));
            if (ret < 0) {
              throw Error(
                `error in input plugin config for ${name}: ${config}`
              );
            }
          });
        }

        // outputs
        if (config.pipeline.filters) {
          config.pipeline.filters.map(({ name, config }) => {
            const filter = this.service.filter(String(name));
            if (filter < 0) {
              throw Error(`error in filter plugin name : ${name}`);
            }
            const ret = this.service.filter_set(filter, ...flb_flatten(config));
            if (ret < 0) {
              throw Error(
                `error in filter plugin config for ${name}: ${config}`
              );
            }
          });
        }

        // filters
        if (config.pipeline.outputs) {
          config.pipeline.outputs.map(({ name, config }) => {
            const output = this.service.output(String(name));
            if (output < 0) {
              throw Error(`error in output plugin name : ${name}`);
            }
            const ret = this.service.output_set(output, ...flb_flatten(config));
            if (ret < 0) {
              throw Error(
                `error in output plugin config for ${name}: ${config}`
              );
            }
          });
        }
      }
    } else {
      const stdout_output = this.service.output("stdout");
      this.service.output_set(stdout_output, "match", "*");
    }

    this.service.start();
  }

  log(data) {
    const data_str = JSON.stringify([Date.now(), data]);
    this.service.lib_push(this.lib_input, data_str);
  }
}

module.exports = new FluentBit();
