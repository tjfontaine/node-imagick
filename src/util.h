#ifndef IMAGICK_UTIL_H
#define IMAGICK_UTIL_H

#include <string.h>

#include <node.h>
#include <node_buffer.h>

#define IMAGICK_PROTOTYPE(templ, name) NODE_SET_PROTOTYPE_METHOD(templ, #name, name)
#define IMAGICK_P(name) static v8::Handle<v8::Value> name (const v8::Arguments &args);

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

inline const v8::Handle<v8::Value> throw_exception(const Magick::Exception &ex)
{
  return v8::ThrowException(v8::Exception::TypeError(v8::String::New(ex.what())));
}

template <typename T, typename Y>
Y _from_obj(const v8::Local<v8::Value> &arg)
{
  T *t = node::ObjectWrap::Unwrap<T>(arg->ToObject());
  return t->opaque_;
}
#endif
