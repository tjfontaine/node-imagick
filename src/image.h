#ifndef IMAGICK_IMAGE_H
#define IMAGICK_IMAGE_H

#include <node.h>
#include <Magick++.h>

#include "util.h"

#define IMAGICK_VOID_DEFINITION(field) \
Handle<Value> \
ImagickImage::field (const Arguments &args) { \
  HandleScope scope; \
  if (args.Length() > 0) \
  { \
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New(#field" does not take any arguments"))); \
  } \
  ImagickImage *image = IUNWRAP(args.This()); \
  try { \
    image->image_.field (); \
    return args.This(); \
  } catch (Magick::Error &error) { \
    return throw_exception(error); \
  } \
}

#define IMAGICK_GEOM_DEFINITION(field) \
Handle<Value> \
ImagickImage::field (const Arguments &args) { \
  HandleScope scope; \
  ENSURE_INSTANCE(Geometry, 0); \
  ImagickImage *image = IUNWRAP(args.This()); \
  ImagickGeometry *geom = GUNWRAP(args[0]->ToObject()); \
  try { \
    image->image_.field (geom->opaque_); \
    return args.This(); \
  } catch (Magick::Error &error) { \
    return throw_exception(error); \
  } \
}

#define IUNWRAP(arg) ObjectWrap::Unwrap<ImagickImage>(arg)

class ImagickImage : public node::ObjectWrap
{
public:
  Magick::Image image_;
  static void Initialize(v8::Handle<v8::Object> target);
  static v8::Handle<v8::Value> New(const v8::Arguments &args);
  static v8::Handle<v8::Value> NotImplemented(const v8::Arguments &args);
  static v8::Handle<v8::Value> readSync(const v8::Arguments &args);
  static v8::Handle<v8::Value> writeSync(const v8::Arguments &args);
  IMAGICK_P(rotate)
  
  /* Void Calls */
  IMAGICK_P(despeckle)
  IMAGICK_P(enhance)
  IMAGICK_P(equalize)
  IMAGICK_P(erase)
  IMAGICK_P(flip);
  IMAGICK_P(flop);
  IMAGICK_P(magnify);
  IMAGICK_P(minify);
  IMAGICK_P(modifyImage);
  IMAGICK_P(normalize);
  IMAGICK_P(trim);

  /* Geom Calls */
  IMAGICK_P(border);
  IMAGICK_P(chop);
  IMAGICK_P(crop);
  IMAGICK_P(sample);
  IMAGICK_P(scale);
  IMAGICK_P(shave);
  IMAGICK_P(zoom);

  IMAGICK_P(adaptiveThreshold);
};

#endif
