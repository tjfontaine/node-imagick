#ifndef IMAGICK_IMAGE_H
#define IMAGICK_IMAGE_H

#include <node.h>
#include <Magick++.h>

#define IMAGICK_VOID_DEFINITION(field) \
Handle<Value> \
ImagickImage::field (const Arguments &args) { \
  HandleScope scope; \
  ImagickImage *image = ObjectWrap::Unwrap<ImagickImage>(args.This()); \
  try { \
    image->image_.field (); \
    return args.This(); \
  } catch (Magick::Error &error) { \
    return throw_exception(error); \
  } \
}

#define IMAGICK_PROTOTYPE(templ, name) NODE_SET_PROTOTYPE_METHOD(templ, #name, name)
#define IMAGICK_P(name) static v8::Handle<v8::Value> name(const v8::Arguments &args);

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
};

#endif
