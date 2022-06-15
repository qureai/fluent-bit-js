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

  Napi::Value service_set(const Napi::CallbackInfo &info);

  Napi::Value input(const Napi::CallbackInfo &info);
  Napi::Value input_set(const Napi::CallbackInfo &info);

  Napi::Value output(const Napi::CallbackInfo &info);
  Napi::Value output_set(const Napi::CallbackInfo &info);

  Napi::Value filter(const Napi::CallbackInfo &info);
  Napi::Value filter_set(const Napi::CallbackInfo &info);

  Napi::Value start(const Napi::CallbackInfo &info);
  Napi::Value lib_push(const Napi::CallbackInfo &info);
  Napi::Value destroy(const Napi::CallbackInfo &info);
};

Napi::Object FluentBit::Init(Napi::Env env, Napi::Object exports)
{
  Napi::Function func =
      DefineClass(env,
                  "FluentBit",
                  {
                      InstanceMethod("service_set", &FluentBit::service_set),

                      InstanceMethod("input", &FluentBit::input),
                      InstanceMethod("input_set", &FluentBit::input_set),

                      InstanceMethod("output", &FluentBit::output),
                      InstanceMethod("output_set", &FluentBit::output_set),

                      InstanceMethod("filter", &FluentBit::filter),
                      InstanceMethod("filter_set", &FluentBit::filter_set),

                      InstanceMethod("start", &FluentBit::start),
                      InstanceMethod("lib_push", &FluentBit::lib_push),
                      InstanceMethod("destroy", &FluentBit::destroy),

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

Napi::Value FluentBit::service_set(const Napi::CallbackInfo &info)
{
  // ref: https://github.com/fluent/fluent-bit/issues/1776#issuecomment-561071592
  Napi::Env env = info.Env();
  if ((info.Length() < 1) || (info.Length() % 2 == 1))
  {
    Napi::TypeError::New(env, "Wrong number of arguments. Should be even")
        .ThrowAsJavaScriptException();
    return Napi::Value();
  }

  const int num_keys = info.Length() / 2;
  int final_return = 0;
  for (int key_idx = 0; key_idx < num_keys; key_idx++)
  {
    std::string key = info[2 * key_idx].As<Napi::String>();
    std::string value = info[2 * key_idx + 1].As<Napi::String>();
    const int ret = flb_service_set(this->context, key.c_str(), value.c_str(), NULL);
    if (ret < 0)
    {
      final_return = ret;
    }
  }
  return Napi::Number::New(env, final_return);
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
  std::string name = info[0].As<Napi::String>();
  const int in_ffd = flb_input(this->context, name.c_str(), NULL);
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

  const int in_ffd = info[0].As<Napi::Number>().Int32Value();
  const int num_keys = (info.Length() - 1) / 2;
  int final_return = 0;
  for (int key_idx = 0; key_idx < num_keys; key_idx++)
  {
    std::string key = info[2 * key_idx + 1].As<Napi::String>();
    std::string value = info[2 * key_idx + 2].As<Napi::String>();
    const int ret = flb_input_set(this->context, in_ffd, key.c_str(), value.c_str(), NULL);
    if (ret < 0)
    {
      final_return = ret;
    }
  }
  return Napi::Number::New(env, final_return);
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

  std::string name = info[0].As<Napi::String>();
  const int out_ffd = flb_output(this->context, name.c_str(), NULL);
  return Napi::Number::New(env, out_ffd);
}

Napi::Value FluentBit::output_set(const Napi::CallbackInfo &info)
{
  // ref: https://github.com/fluent/fluent-bit/issues/1776#issuecomment-561071592
  Napi::Env env = info.Env();
  if ((info.Length() < 1) || (info.Length() % 2 == 0))
  {
    Napi::TypeError::New(env, "Wrong number of arguments. Should be odd")
        .ThrowAsJavaScriptException();
    return Napi::Value();
  }

  const int in_ffd = info[0].As<Napi::Number>().Int32Value();
  const int num_keys = (info.Length() - 1) / 2;
  int final_return = 0;
  for (int key_idx = 0; key_idx < num_keys; key_idx++)
  {
    std::string key = info[2 * key_idx + 1].As<Napi::String>();
    std::string value = info[2 * key_idx + 2].As<Napi::String>();
    const int ret = flb_output_set(this->context, in_ffd, key.c_str(), value.c_str(), NULL);
    if (ret < 0)
    {
      final_return = ret;
    }
  }
  return Napi::Number::New(env, final_return);
}

Napi::Value FluentBit::filter(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  if (info.Length() != 1)
  {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return Napi::Value();
  }

  std::string name = info[0].As<Napi::String>();
  const int out_ffd = flb_filter(this->context, name.c_str(), NULL);
  return Napi::Number::New(env, out_ffd);
}

Napi::Value FluentBit::filter_set(const Napi::CallbackInfo &info)
{
  // ref: https://github.com/fluent/fluent-bit/issues/1776#issuecomment-561071592
  Napi::Env env = info.Env();
  if ((info.Length() < 1) || (info.Length() % 2 == 0))
  {
    Napi::TypeError::New(env, "Wrong number of arguments. Should be odd")
        .ThrowAsJavaScriptException();
    return Napi::Value();
  }

  const int in_ffd = info[0].As<Napi::Number>().Int32Value();
  const int num_keys = (info.Length() - 1) / 2;
  int final_return = 0;
  for (int key_idx = 0; key_idx < num_keys; key_idx++)
  {
    std::string key = info[2 * key_idx + 1].As<Napi::String>();
    std::string value = info[2 * key_idx + 2].As<Napi::String>();
    const int ret = flb_filter_set(this->context, in_ffd, key.c_str(), value.c_str(), NULL);
    if (ret < 0)
    {
      final_return = ret;
    }
  }
  return Napi::Number::New(env, final_return);
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
  int ret = flb_start(this->context);
  return Napi::Number::New(env, ret);
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

  const int in_ffd = info[0].As<Napi::Number>().Int32Value();
  std::string data = info[1].As<Napi::String>();

  int result = flb_lib_push(this->context, in_ffd, data.c_str(), strlen(data.c_str()));
  return Napi::Number::New(env, result);
}

Napi::Value FluentBit::destroy(const Napi::CallbackInfo &info)
{
  std::cout << "Entering destroy..." << std::endl;
  flb_stop(this->context);
  flb_destroy(this->context);
  this->context = NULL;
  std::cout << "Exiting destroy..." << std::endl;
  return Napi::Value();
}

FluentBit::~FluentBit()
{
  std::cout << "Entering deconstructor..." << std::endl;
  if (this->context != NULL)
  {
    flb_stop(this->context);
    flb_destroy(this->context);
  }
  std::cout << "Exiting deconstructor..." << std::endl;
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  return FluentBit::Init(env, exports);
}

NODE_API_MODULE(fluentbit, Init)