// Not using any header file. Can refactor when needed
#include <fluent-bit.h>
#include <napi.h>

class FluentBit : public Napi::ObjectWrap<FluentBit>
{
private:
  flb_ctx_t *context;

public:
  FluentBit(const Napi::CallbackInfo &info);
  ~FluentBit();
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Function GetClass(Napi::Env);

  Napi::Value input(const Napi::CallbackInfo &info);
};

FluentBit::FluentBit(const Napi::CallbackInfo &info) : Napi::ObjectWrap<FluentBit>(info)
{
  this->context = flb_create();
  // TODO: Raise exception if null
}

Napi::Value FluentBit::input(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  if (info.Length() != 1)
  {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return Napi::Value();
  }

  if (!info[0].IsString())
  {
    Napi::TypeError::New(env, "Input plugin name must be string")
        .ThrowAsJavaScriptException();
    return Napi::Value();
  }
  const char *name = info[0].As<Napi::String>().Utf8Value().c_str();

  flb_input(this->context, name, NULL);
  return Napi::Value();
}

Napi::Function FluentBit::GetClass(const Napi::Env env)
{
  return DefineClass(
      env,
      "FluentBit",
      {
          FluentBit::InstanceMethod("input", &FluentBit::input),
      });
}

Napi::Object FluentBit::Init(Napi::Env env, Napi::Object exports)
{
  Napi::Function func =
      DefineClass(env,
                  "FluentBit",
                  {
                      InstanceMethod("input", &FluentBit::input),
                  });
    Napi::String name = Napi::String::New(env, "FluentBit");

  exports.Set(name, func);
  return exports;
}

FluentBit::~FluentBit()
{
  if (this->context != NULL)
  {
    // this->stop();
    flb_destroy(this->context);
  }
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  return FluentBit::Init(env, exports);

}

NODE_API_MODULE(fluentbit, Init)