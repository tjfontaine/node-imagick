#ifndef IMAGICK_IMAGE_H
#define IMAGICK_IMAGE_H

#include <node.h>
#include <Magick++.h>

class ImagickImage : public node::ObjectWrap
{
public:
  Magick::Image image_;
  static void Initialize(v8::Handle<v8::Object> target);
  static v8::Handle<v8::Value> New(const v8::Arguments &args);
  static v8::Handle<v8::Value> NotImplemented(const v8::Arguments &args);
  static v8::Handle<v8::Value> readSync(const v8::Arguments &args);
  static v8::Handle<v8::Value> writeSync(const v8::Arguments &args);
  static v8::Handle<v8::Value> rotate(const v8::Arguments &args);
};

#endif
