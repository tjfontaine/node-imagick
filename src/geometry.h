#ifndef IMAGICK_GEOMETRY_H
#define IMAGICK_GEOMETRY_H

#include <node.h>
#include <Magick++.h>
#include "util.h"

class ImagickGeometry : public node::ObjectWrap
{
public:
  Magick::Geometry opaque_;
  static void Initialize(v8::Handle<v8::Object> target);
  static v8::Handle<v8::Value> New(const v8::Arguments &args);
  static v8::Handle<v8::Value> toString(const v8::Arguments &args);
  static v8::Persistent<v8::FunctionTemplate> Klass;
};


#define ISG(arg) ImagickGeometry::Klass->HasInstance(arg)
#define GUNWRAP(arg) ObjectWrap::Unwrap<ImagickGeometry>(arg)

inline const Magick::Geometry from_obj(const v8::Local<v8::Value> &arg)
{
  return _from_obj<ImagickGeometry, Magick::Geometry>(arg);
}

#endif
