// Not using any header file. Can refactor when needed
#include <fluent-bit.h>
#include <napi.h>

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  return exports;
}

NODE_API_MODULE(fluentbit, Init)