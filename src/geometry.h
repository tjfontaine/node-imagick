#ifndef IMAGICK_GEOMETRY_H
#define IMAGICK_GEOMETRY_H

#include <node.h>
#include <Magick++.h>

class ImagickGeometry : public node::ObjectWrap
{
public:
  Magick::Geometry geo_;
  static void Initialize(v8::Handle<v8::Object> target);
  static v8::Handle<v8::Value> New(const v8::Arguments &args);
  static v8::Handle<v8::Value> toString(const v8::Arguments &args);
};

#endif
