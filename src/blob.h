#ifndef IMAGICK_BLOB_H
#define IMAGICK_BLOB_H

#include <node.h>
#include <Magick++.h>
#include "util.h"

class ImagickBlob : public node::ObjectWrap
{
public:
  Magick::Blob opaque_;
  static void Initialize(v8::Handle<v8::Object> target);
  static v8::Handle<v8::Value> New(const v8::Arguments &args);
  static v8::Handle<v8::Value> length(const v8::Arguments &args);
  static v8::Handle<v8::Value> toBuffer(const v8::Arguments &args);
  static v8::Persistent<v8::FunctionTemplate> Klass;
};

#define ISB(arg) ImagickBlob::Klass->HasInstance(arg)
#define BUNWRAP(arg) ObjectWrap::Unwrap<ImagickBlob>(arg)

inline const v8::Local<v8::Object> from_blob(const Magick::Blob &blob)
{
  node::Buffer *slowBuffer = node::Buffer::New(blob.length());
  memcpy(node::Buffer::Data(slowBuffer), blob.data(), blob.length());

  v8::Local<v8::Object> globalObj = v8::Context::GetCurrent()->Global();
  v8::Local<v8::Function> bufferConstructor = v8::Local<v8::Function>::Cast(globalObj->Get(v8::String::New("Buffer")));

  v8::Handle<v8::Value> constructorArgs[3] = {
    slowBuffer->handle_,
    v8::Integer::New(blob.length()),
    v8::Integer::New(0)
  };

  v8::Local<v8::Object> actualBuffer = bufferConstructor->NewInstance(3, constructorArgs);
  return actualBuffer;
}

inline const Magick::Blob to_blob(const v8::Local<v8::Value> &arg)
{
  v8::Local<v8::Object> buffer_obj = arg->ToObject();
  return Magick::Blob(node::Buffer::Data(buffer_obj), node::Buffer::Length(buffer_obj));
}

#endif
