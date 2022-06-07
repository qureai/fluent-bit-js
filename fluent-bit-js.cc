// Not using any header file. Can refactor when needed
#include <fluent-bit.h>
#include <napi.h>
#include <iostream>
#include <typeinfo>

#define JSON_2   "[1449505620, {\"key1\": \"some new value\"}]"


class FluentBit : public Napi::ObjectWrap<FluentBit>
{
private:
  flb_ctx_t *context;

public:
  FluentBit(const Napi::CallbackInfo &info);
  ~FluentBit();
  static Napi::Object Init(Napi::Env env, Napi::Object exports);

  Napi::Value input(const Napi::CallbackInfo &info);
  Napi::Value output(const Napi::CallbackInfo &info);
  Napi::Value start(const Napi::CallbackInfo &info);
  Napi::Value lib_push(const Napi::CallbackInfo &info);
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

  const int in_ffd = flb_input(this->context, name, NULL);
  return Napi::Number::New(env, in_ffd);
}

Napi::Value FluentBit::output(const Napi::CallbackInfo &info)
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
    Napi::TypeError::New(env, "Output plugin name must be string")
        .ThrowAsJavaScriptException();
    return Napi::Value();
  }
  const char *name = info[0].As<Napi::String>().Utf8Value().c_str();

  const int out_ffd = flb_output(this->context, name, NULL);
  return Napi::Number::New(env, out_ffd);
}


Napi::Value FluentBit::start(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  if (info.Length() != 0)
  {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return Napi::Value();
  }
  flb_start(this->context);
  return Napi::Value();
}

Napi::Value FluentBit::lib_push(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  if (info.Length() != 3)
  {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return Napi::Value();
  }
  const int in_ffd = info[0].As<Napi::Number>().Int32Value();
  const char *data = "hellow world"; //info[1].As<Napi::String>().Utf8Value().c_str();
  int result = flb_lib_push(this->context, in_ffd, data, strlen(data));
  printf("data %s", data);
  // std::cout << "size of JSON_2 " << sizeof(JSON_2) << " " << typeid(JSON_2).name();
  return Napi::Number::New(env, result);
}

Napi::Object FluentBit::Init(Napi::Env env, Napi::Object exports)
{
  Napi::Function func =
      DefineClass(env,
                  "FluentBit",
                  {
                      InstanceMethod("input", &FluentBit::input),
                      InstanceMethod("output", &FluentBit::output),
                      InstanceMethod("start", &FluentBit::start),
                      InstanceMethod("lib_push", &FluentBit::lib_push),
                  });
    Napi::String name = Napi::String::New(env, "FluentBit");

  exports.Set(name, func);
  return exports;
}

FluentBit::~FluentBit()
{
  if (this->context != NULL)
  {
    flb_stop(this->context);
    flb_destroy(this->context);
  }
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  return FluentBit::Init(env, exports);

}

NODE_API_MODULE(fluentbit, Init)