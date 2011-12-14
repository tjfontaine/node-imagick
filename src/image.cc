#include <string.h>

#include <node.h>
#include <node_buffer.h>

#include <Magick++.h>

#include "util.h"
#include "image.h"

using namespace v8;
using namespace node;

Handle<Value>
ImagickImage::New(const Arguments &args)
{
  HandleScope scope;

  ImagickImage *image = new ImagickImage();
  image->Wrap(args.This());

  return args.This();
}

Handle<Value>
ImagickImage::NotImplemented(const Arguments &args)
{
  return ThrowException(Exception::TypeError(String::New("This function not yet implemented")));
}

Handle<Value>
ImagickImage::readSync(const Arguments &args)
{
  HandleScope scope;
  ImagickImage *image = ObjectWrap::Unwrap<ImagickImage>(args.This());

  if(args[0]->IsString())
  {
    image->image_.read(to_string(args[0]));
  }
  else if(Buffer::HasInstance(args[0]))
  {
    image->image_.read(to_blob(args[0]));
  }
  else
  {
    return ThrowException(Exception::TypeError(String::New("Reading from this type not implemented")));
  }

  return args.This();
}

Handle<Value>
ImagickImage::writeSync(const Arguments &args)
{
  HandleScope scope;
  ImagickImage *image = ObjectWrap::Unwrap<ImagickImage>(args.This());
  Local<Value> ret;

  if (args.Length() == 0)
  {
    Magick::Blob bl;
    image->image_.write(&bl);
    ret = from_blob(bl);
  }
  else if(args[0]->IsString())
  {
    image->image_.write(to_string(args[0]));
    ret = args.This();
  }
  else
  {
    return ThrowException(Exception::TypeError(String::New("Writing to this type not implemented")));
  }

  return ret;
}

Handle<Value>
ImagickImage::rotate(const Arguments &args)
{
  HandleScope scope;
  ImagickImage *image = ObjectWrap::Unwrap<ImagickImage>(args.This());

  if(args[0]->IsNumber())
  {
    image->image_.rotate(args[0]->NumberValue());
  }

  return args.This();
}

void
ImagickImage::Initialize(Handle<Object> target)
{
  HandleScope scope;
  Local<FunctionTemplate> t = FunctionTemplate::New(New);
  t->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_PROTOTYPE_METHOD(t, "adaptiveThreshold", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "addNoise", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "addNoiseChannel", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "affineTransform", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "annotate", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "blur", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "blurChannel", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "border", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "cdl", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "channel", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "charcol", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "chop", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "colorize", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "comment", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "compare", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "composite", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "contrast", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "convolve", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "crop", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "cycleColormap", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "despeckle", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "display", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "distort", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "draw", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "edge", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "emboss", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "enhance", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "equalize", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "erase", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "extent", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "flip", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "floodFillColor", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "floodFillTexture", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "flop", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "frame", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "fx", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "gamma", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "gaussianBlur", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "gaussianBlurChannel", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "haldClut", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "implode", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "inverseFourierTransform", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "label", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "level", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "levelChannel", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "magnify", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "map", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "matteFloodfill", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "medianFilter", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "minify", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "modifyImage", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "modulate", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "motionBlur", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "negate", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "normalize", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "oilPaint", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "opacity", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "opaque", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "ping", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "process", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "quantize", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "raise", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "read", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "readSync", readSync);
  NODE_SET_PROTOTYPE_METHOD(t, "reduceNoise", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "randomThreshold", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "randomThresholdChannel", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "roll", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "rotate", rotate);
  NODE_SET_PROTOTYPE_METHOD(t, "sample", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "scale", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "segment", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "shade", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "sharpen", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "sharpenChannel", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "shave", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "shear", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "solarize", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "splice", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "spread", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "stegano", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "sparseColor", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "statistics", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "stereo", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "swirl", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "texture", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "transform", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "transparent", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "trim", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "unsharpmask", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "unsharpmaskChannel", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "wave", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "write", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "writeSync", writeSync);
  NODE_SET_PROTOTYPE_METHOD(t, "zoom", NotImplemented);
  //NODE_SET_PROTOTYPE_METHOD(t, "", NotImplemented);

  target->Set(String::NewSymbol("image"), t->GetFunction());
}
