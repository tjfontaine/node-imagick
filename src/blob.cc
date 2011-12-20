#include "blob.h"
#include "util.h"

#define BLOB_LENGTH String::NewSymbol("length")

using namespace v8;
using namespace node;

Handle<Value>
ImagickBlob::New(const Arguments &args)
{
  HandleScope scope;
  ImagickBlob *bl = new ImagickBlob();

  if (args.Length() > 0 && Buffer::HasInstance(args[0]))
  {
    Local<Object> buff = args[0]->ToObject();
    bl->opaque_ = Magick::Blob(Buffer::Data(buff), Buffer::Length(buff));
  }

  bl->Wrap(args.This());
  return args.This();
}

Handle<Value>
ImagickBlob::toBuffer(const Arguments &args)
{
  HandleScope scope;
  ImagickBlob *bl = BUNWRAP(args.This());
  Local<Value> buff = from_blob(bl->opaque_);
  return scope.Close(buff);
}

static Handle<Value>
NumberGetter(Local<String> property, const AccessorInfo& info)
{
  HandleScope scope;
  return scope.Close(Number::New(BUNWRAP(info.This())->opaque_.length()));
}

Persistent<FunctionTemplate> ImagickBlob::Klass;

void
ImagickBlob::Initialize(Handle<Object> target)
{
  HandleScope scope;
  Local<FunctionTemplate> t = FunctionTemplate::New(New);
  Klass = Persistent<FunctionTemplate>::New(t);
  Klass->InstanceTemplate()->SetInternalFieldCount(1);

  IMAGICK_PROTOTYPE(t, toBuffer);

  Klass->PrototypeTemplate()->SetAccessor(BLOB_LENGTH, NumberGetter);

  target->Set(String::NewSymbol("blob"), Klass->GetFunction());
}
