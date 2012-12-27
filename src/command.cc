#include <node.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <pthread.h>

#include <wand/MagickWand.h>

using namespace v8;
using namespace node;

static ImageInfo *image_info;
static ExceptionInfo *exception_info;

static Handle<Value> dispatch(const Arguments& args, MagickCommand cmd, const char *command) {
  HandleScope scope;

  Local<Array> argv_handle = Local<Array>::Cast(args[0]);

  Local<Function> cb = Local<Function>::Cast(args[1]);

  int argc = argv_handle->Length();
  char **argv;

  int argv_length = argc + 1;

  argv = new char*[argv_length]; // heap allocated to detect errors

  int i;

  argv[0] = strdup(command);

  for (i = 0; i < argc; i++) {
    String::Utf8Value arg(argv_handle->Get(Integer::New(i))->ToString());
    argv[i + 1] = strdup(*arg);
  }

  GetExceptionInfo(exception_info);
  GetImageInfo(image_info);

  char *metadata = NULL;

  MagickBooleanType result = cmd(image_info, argv_length, argv, &metadata, exception_info);

  Local<Value> cb_argv[2];

  cb_argv[0] = Local<Value>::New(Undefined());
  if (exception_info->reason != NULL)
    cb_argv[0] = String::New(exception_info->reason);

  cb_argv[1] = Local<Value>::New(Undefined());
  if (metadata != NULL)
    cb_argv[1] = String::New(metadata);

  if (metadata != NULL)
    DestroyString(metadata);

  for (i = 0; i < argv_length; i++) {
    free(argv[i]);
  }

  TryCatch try_catch;

  cb->Call(Context::GetCurrent()->Global(), 2, cb_argv);

  if(try_catch.HasCaught()) {
    FatalException(try_catch);
  }

  return Undefined();
}

static Handle<Value> Mogrify(const Arguments& args) {
  HandleScope scope;
  return dispatch(args, MogrifyImageCommand, "mogrify");
}

static Handle<Value> Convert(const Arguments& args) {
  HandleScope scope;
  return dispatch(args, ConvertImageCommand, "convert");
}

static Handle<Value> Composite(const Arguments& args) {
  HandleScope scope;
  return dispatch(args, CompositeImageCommand, "composite");
}

static Handle<Value> Identify(const Arguments& args) {
  HandleScope scope;
  return dispatch(args, IdentifyImageCommand, "identify");
}

static Handle<Value> Compare(const Arguments& args) {
  HandleScope scope;
  return dispatch(args, CompareImageCommand, "compare");
}

static Handle<Value> Conjure(const Arguments& args) {
  HandleScope scope;
  return dispatch(args, ConjureImageCommand, "conjure");
}

static Handle<Value> Stream(const Arguments& args) {
  HandleScope scope;
  return dispatch(args, StreamImageCommand, "stream");
}

static Handle<Value> Import(const Arguments& args) {
  HandleScope scope;
  return dispatch(args, ImportImageCommand, "import");
}

static Handle<Value> Display(const Arguments& args) {
  HandleScope scope;
  return dispatch(args, DisplayImageCommand, "display");
}

static Handle<Value> Animate(const Arguments& args) {
  HandleScope scope;
  return dispatch(args, AnimateImageCommand, "animate");
}

static Handle<Value> Montage(const Arguments& args) {
  HandleScope scope;
  return dispatch(args, MontageImageCommand, "montage");
}

extern "C" void
CommandInit (Handle<Object> target)
{
  HandleScope scope;
  NODE_SET_METHOD(target, "Mogrify", Mogrify);
  NODE_SET_METHOD(target, "Convert", Convert);
  NODE_SET_METHOD(target, "Composite", Composite);
  NODE_SET_METHOD(target, "Identify", Identify);
  NODE_SET_METHOD(target, "Compare", Compare);
  NODE_SET_METHOD(target, "Conjure", Conjure);
  NODE_SET_METHOD(target, "Stream", Stream);
  NODE_SET_METHOD(target, "Import", Import);
  NODE_SET_METHOD(target, "Display", Display);
  NODE_SET_METHOD(target, "Animate", Animate);
  NODE_SET_METHOD(target, "Montage", Montage);
  
  NODE_SET_METHOD(target, "mogrify", Mogrify);
  NODE_SET_METHOD(target, "convert", Convert);
  NODE_SET_METHOD(target, "composite", Composite);
  NODE_SET_METHOD(target, "identify", Identify);
  NODE_SET_METHOD(target, "compare", Compare);
  NODE_SET_METHOD(target, "conjure", Conjure);
  NODE_SET_METHOD(target, "stream", Stream);
  NODE_SET_METHOD(target, "import", Import);
  NODE_SET_METHOD(target, "display", Display);
  NODE_SET_METHOD(target, "animate", Animate);
  NODE_SET_METHOD(target, "montage", Montage);

  MagickCoreGenesis("/dev/null", MagickTrue);

  image_info = AcquireImageInfo();
  exception_info = AcquireExceptionInfo();
}
