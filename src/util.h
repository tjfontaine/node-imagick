#ifndef IMAGICK_UTIL_H
#define IMAGICK_UTIL_H

#include <string.h>

#include <node.h>
#include <node_buffer.h>

#define IMAGICK_PROTOTYPE(templ, name) NODE_SET_PROTOTYPE_METHOD(templ, #name, name)
#define IMAGICK_P(name) static v8::Handle<v8::Value> name (const v8::Arguments &args);

#define THROW_STRING(message) v8::ThrowException(v8::Exception::TypeError(v8::String::New((message))))

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
  return THROW_STRING(ex.what());
}

template <typename T, typename Y>
Y _from_obj(const v8::Local<v8::Value> &arg)
{
  T *t = node::ObjectWrap::Unwrap<T>(arg->ToObject());
  return t->opaque_;
}

#define ENSURE(type, arg) \
do { \
  if (!args[arg]->Is ## type()) \
  { \
    return THROW_STRING("Argument " #arg  " must be of type " #type); \
  } \
} while(0)

#define ENSURE_INSTANCE(type, arg) \
do { \
  if (!Imagick ## type::Klass->HasInstance(args[arg]->ToObject())) \
  { \
    return THROW_STRING("Argument " #arg  " must be of type " #type); \
  } \
} while(0)

#endif

#define TRY_CATCH(stmt) \
do { \
  try {\
    stmt; \
  } catch (Magick::Error &error) { \
    return throw_exception(error); \
  } \
} while (0)
