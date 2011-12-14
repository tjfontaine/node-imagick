#ifndef IMAGICK_UTIL_H
#define IMAGICK_UTIL_H

#include <node.h>

template <typename T>
T _to_string(v8::Handle<v8::String> str)
{
  v8::String::Utf8Value a(str);
  return T(*a);
}

inline const std::string to_string(v8::Local<v8::String> str)
{
  return _to_string<std::string>(str);
}

inline const std::string to_string(const v8::Local<v8::Value> &arg)
{
  return to_string(arg->ToString());
}

inline const Magick::Blob to_blob(const v8::Local<v8::Value> &arg)
{
  v8::Local<v8::Object> buffer_obj = arg->ToObject();
  return Magick::Blob(node::Buffer::Data(buffer_obj), node::Buffer::Length(buffer_obj));
}

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

inline const v8::Handle<v8::Value> throw_exception(const Magick::Exception &ex)
{
  return v8::ThrowException(v8::Exception::TypeError(v8::String::New(ex.what())));
}
#endif
