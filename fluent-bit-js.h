#include <napi.h>
#include <fluent-bit.h>
using namespace std;

//add number function
int example_function();
Napi::Number example_function_wrapped(const Napi::CallbackInfo& info); //Export API
Napi::Object Init(Napi::Env env, Napi::Object exports);
NODE_API_MODULE(addon, Init)