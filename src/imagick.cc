#include <node.h>
#include <node_events.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <wand/MagickWand.h>

using namespace v8;
using namespace node;

static Handle<Value> dispatch(const Arguments& args, MagickCommand cmd) {
  HandleScope scope;

  Local<Array> argv_handle = Local<Array>::Cast(args[0]);
  int argc = argv_handle->Length();
  int argv_length = argc + 1 + 1;
  char **argv = new char*[argv_length]; // heap allocated to detect errors
  int i;

  for (i = 0; i < argc; i++) {
    String::Utf8Value arg(argv_handle->Get(Integer::New(i))->ToString());
    argv[i] = strdup(*arg);
  }

  MagickBooleanType r = MagickCommandGenesis(AcquireImageInfo(), cmd, argc, argv, NULL, AcquireExceptionInfo());
  return Boolean::New(r == MagickTrue ? 1 : 0);
}

static Handle<Value> Mogrify(const Arguments& args) {
  HandleScope scope;
  return dispatch(args, MogrifyImageCommand);
}

static Handle<Value> Convert(const Arguments& args) {
  HandleScope scope;
  return dispatch(args, ConvertImageCommand);
}

extern "C" void
init (Handle<Object> target)
{
  HandleScope scope;
  NODE_SET_METHOD(target, "Mogrify", Mogrify);
  NODE_SET_METHOD(target, "Convert", Convert);
}
