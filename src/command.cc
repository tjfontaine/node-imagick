#include <node.h>
#include <uv.h>

#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <pthread.h>

#include <wand/MagickWand.h>

using namespace v8;
using namespace node;

struct command_args {
  ImageInfo *ii;
  ExceptionInfo *ei;
  Persistent<Function> cb;
  MagickCommand cmd;
  int argc;
  char **argv;
  char *metadata;
  int result;
};

static void command_args_free(command_args *args) {
  int i;

  for (i = 0; i < args->argc; i++) {
    free(args->argv[i]);
  }

  delete args->argv;

  if (args->metadata != NULL)
    DestroyString(args->metadata);

  DestroyImageInfo(args->ii);
  DestroyExceptionInfo(args->ei);

  delete args;
};

static void DoSyncCall(uv_work_t *req) {
  command_args *args = (command_args *)req->data;
  args->result = args->cmd(args->ii, args->argc, args->argv, &args->metadata, args->ei);
}

static void DoSyncCall_After(uv_work_t *req, int status) {
  HandleScope scope;

  command_args *args = (command_args *)req->data;

  Local<Value> cb_argv[2];

  cb_argv[0] = Local<Value>::New(Boolean::New(args->result));
  if (args->ei->reason != NULL)
    cb_argv[0] = String::New(args->ei->reason);

  if (args->metadata != NULL)
    cb_argv[1] = String::New(args->metadata);
  else
    cb_argv[1] = Local<Value>::New(Undefined());

  TryCatch try_catch;

  args->cb->Call(Context::GetCurrent()->Global(), 2, cb_argv);

  args->cb.Dispose();

  command_args_free(args);
  delete req;

  if(try_catch.HasCaught()) {
    FatalException(try_catch);
  }
}

static Handle<Value> dispatch(const Arguments& args, MagickCommand cmd, const char *command) {
  Local<Array> argv_handle = Local<Array>::Cast(args[0]);

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

  command_args *async_args = new command_args;
  async_args->ii = AcquireImageInfo();
  async_args->ei = AcquireExceptionInfo();
  async_args->cmd = cmd;
  async_args->argc = argv_length;
  async_args->argv = argv;
  async_args->metadata = NULL;
  async_args->cb = Persistent<Function>::New(Local<Function>::Cast(args[1]));

  uv_work_t *req = new uv_work_t;
  req->data = async_args;

  uv_queue_work(uv_default_loop(), req, DoSyncCall, DoSyncCall_After);

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

extern "C" WandExport MagickBooleanType TJCompareImageCommand(
  ImageInfo *, int, char **, char **, ExceptionInfo *);

static Handle<Value> Compare(const Arguments& args) {
  HandleScope scope;
  return dispatch(args, TJCompareImageCommand, "compare");
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

  MagickCoreGenesis("/tmp", MagickFalse);
}
