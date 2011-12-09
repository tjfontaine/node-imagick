#include <node.h>
#include <node_events.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <pthread.h>

#include <wand/MagickWand.h>

using namespace v8;
using namespace node;

struct command_args {
  Persistent<Function> cb;
  MagickCommand cmd;
  MagickBooleanType result;
  int argc;
  char **argv;
};

static void command_args_free (struct command_args *args) {
  int i;
  if (args != NULL) {
    args->cb.Dispose();

    for (i = 0; i < args->argc; i++) {
      if (args->argv[i] != NULL) {
        free(args->argv[i]);
        args->argv[i] = NULL;
      }
    }

    delete args->argv;

    free(args);
  }
}

static void *inner_thread (void *data) {
  struct command_args *args = (struct command_args *)data;
  args->result = MagickCommandGenesis(AcquireImageInfo(), args->cmd, args->argc,
                                      args->argv, NULL, AcquireExceptionInfo());

  return NULL;
}

static int DoSyncCall(eio_req *req) {
  void *res;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setstacksize(&attr, 8192);

  pthread_t thread;

  pthread_create(&thread, &attr, &inner_thread, req->data);
  pthread_join(thread, &res);

  return 0;
}

static int DoSyncCall_After(eio_req *req) {
  HandleScope scope;
  ev_unref(EV_DEFAULT_UC);
  struct command_args *args = (struct command_args *)req->data;

  Local<Value> argv[1];
  argv[0] = Integer::New(args->result);

  TryCatch try_catch;

  args->cb->Call(Context::GetCurrent()->Global(), 1, argv);

  if(try_catch.HasCaught()) {
    FatalException(try_catch);
  }

  command_args_free(args);

  return 0;
}

static Handle<Value> dispatch(const Arguments& args, MagickCommand cmd) {
  HandleScope scope;

  command_args *async_args = (command_args *)malloc(sizeof (struct command_args));

  Local<Array> argv_handle = Local<Array>::Cast(args[0]);

  async_args->cb = Persistent<Function>::New(Local<Function>::Cast(args[1]));
  async_args->argc = argv_handle->Length();
  async_args->cmd = cmd;

  int argv_length = async_args->argc + 1 + 1;

  async_args->argv = new char*[argv_length]; // heap allocated to detect errors

  int i;

  for (i = 0; i < async_args->argc; i++) {
    String::Utf8Value arg(argv_handle->Get(Integer::New(i))->ToString());
    async_args->argv[i] = strdup(*arg);
  }

  eio_custom(DoSyncCall, EIO_PRI_DEFAULT, DoSyncCall_After, async_args);  
  ev_ref(EV_DEFAULT_UC);

  return Undefined();
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
