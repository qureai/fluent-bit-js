// Not using any header file. Can refactor when needed
#include <fluent-bit.h>
#include <napi.h>

class FluentBit : public Napi::ObjectWrap<FluentBit>
{
private:
  flb_ctx_t *context;

public:
  FluentBit(const Napi::CallbackInfo& info);
  ~FluentBit();
  int input(char *name, void *data);
  int output(char *name, void *data);
  int start();
  int stop();
  int push(int in_ffd, void *data, size_t len);
};

FluentBit::FluentBit(const Napi::CallbackInfo& info) : Napi::ObjectWrap<FluentBit>(info) {
  this->context = flb_create();
  // TODO: Raise exception if null
}

int FluentBit::input(char *name, void *data)
{
  return flb_input(this->context, name, data);
}

int FluentBit::output(char *name, void *data)
{
  return flb_output(this->context, name, (flb_lib_out_cb *)data);
}

int FluentBit::start()
{
  return flb_start(this->context);
}

int FluentBit::stop()
{
  return flb_stop(this->context);
}

int FluentBit::push(int in_ffd, void *data, size_t len)
{
  return flb_lib_push(this->context, in_ffd, data, len);
}

FluentBit::~FluentBit()
{
  if (this->context != NULL)
  {
    this->stop();
    flb_destroy(this->context);
  }
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  return exports;
}

NODE_API_MODULE(fluentbit, Init)