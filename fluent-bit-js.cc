// Not using any header file. Can refactor when needed
#include <fluent-bit.h>
#include <napi.h>
#include <iostream>

class FluentBit : public Napi::ObjectWrap<FluentBit>
{
private:
  flb_ctx_t *context;

public:
  FluentBit(const Napi::CallbackInfo &info);
  ~FluentBit();
  static Napi::Object Init(Napi::Env env, Napi::Object exports);

  Napi::Value input(const Napi::CallbackInfo &info);
  Napi::Value input_set(const Napi::CallbackInfo &info);
  Napi::Value output(const Napi::CallbackInfo &info);
  Napi::Value start(const Napi::CallbackInfo &info);
  Napi::Value lib_push(const Napi::CallbackInfo &info);
};


Napi::Object FluentBit::Init(Napi::Env env, Napi::Object exports)
{
  Napi::Function func =
      DefineClass(env,
                  "FluentBit",
                  {
                      InstanceMethod("input", &FluentBit::input),
                      InstanceMethod("input_set", &FluentBit::input_set),
                      InstanceMethod("output", &FluentBit::output),
                      InstanceMethod("start", &FluentBit::start),
                      InstanceMethod("lib_push", &FluentBit::lib_push),
                  });
  Napi::String name = Napi::String::New(env, "FluentBit");
  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  env.SetInstanceData(constructor);

  exports.Set(name, func);
  return exports;
}

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

Napi::Value FluentBit::input_set(const Napi::CallbackInfo &info)
{
  // ref: https://github.com/fluent/fluent-bit/issues/1776#issuecomment-561071592
  Napi::Env env = info.Env();
  if ((info.Length() < 1) || (info.Length() % 2 == 0))
  {
    Napi::TypeError::New(env, "Wrong number of arguments. Should be odd")
        .ThrowAsJavaScriptException();
    return Napi::Value();
  }
  if (!info[0].IsNumber())
  {
    Napi::TypeError::New(env, "Input plugin id must be a number")
        .ThrowAsJavaScriptException();
    return Napi::Value();
  }

  const int in_ffd = info[0].As<Napi::Number>().Int32Value();
  
  std::cout << "hi" << info << std::endl;

  const int num_keys = (info.Length() - 1) / 2;

  for (int key_idx = 0; key_idx < num_keys; key_idx++)
  {
    /* code */
    const char *key = info[2 * key_idx + 1].As<Napi::String>().Utf8Value().c_str();
    const char *value = info[2 * key_idx + 2].As<Napi::String>().Utf8Value().c_str();
    std::cout << key_idx << " " << key << " " << value << std::endl;
  }

  // const int ret = flb_input_set(this->context, in_ffd, key, value, NULL);
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
  if (info.Length() != 2)
  {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return Napi::Value();
  }
  if (!info[0].IsNumber())
  {
    Napi::TypeError::New(env, "Input plugin id must be a number")
        .ThrowAsJavaScriptException();
    return Napi::Value();
  }

  if (!info[1].IsString())
  {
    Napi::TypeError::New(env, "Input data must be a string")
        .ThrowAsJavaScriptException();
    return Napi::Value();
  }

  const int in_ffd = info[0].As<Napi::Number>().Int32Value();
  std::string data = info[1].As<Napi::String>();

  int result = flb_lib_push(this->context, in_ffd, data.c_str(), strlen(data.c_str()));

  return Napi::Number::New(env, result);
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