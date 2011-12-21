#include <node.h>

#include <Magick++.h>

#include "util.h"
#include "image.h"
#include "geometry.h"
#include "blob.h"

using namespace v8;
using namespace node;

IMAGICK_VOID_DEFINITION(despeckle)
IMAGICK_VOID_DEFINITION(enhance)
IMAGICK_VOID_DEFINITION(equalize)
IMAGICK_VOID_DEFINITION(erase)
IMAGICK_VOID_DEFINITION(flip);
IMAGICK_VOID_DEFINITION(flop);
IMAGICK_VOID_DEFINITION(magnify);
IMAGICK_VOID_DEFINITION(minify);
IMAGICK_VOID_DEFINITION(modifyImage);
IMAGICK_VOID_DEFINITION(normalize);
IMAGICK_VOID_DEFINITION(trim);

IMAGICK_GEOM_DEFINITION(border);
IMAGICK_GEOM_DEFINITION(chop);
IMAGICK_GEOM_DEFINITION(crop);
IMAGICK_GEOM_DEFINITION(sample);
IMAGICK_GEOM_DEFINITION(scale);
IMAGICK_GEOM_DEFINITION(shave);
IMAGICK_GEOM_DEFINITION(zoom);

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
  Local<String> s = String::New("This function not yet implemented: ");
  Local<String> f = args.Callee()->GetName()->ToString();
  return ThrowException(Exception::TypeError(String::Concat(s, f)));
}

Handle<Value>
ImagickImage::readSync(const Arguments &args)
{
  HandleScope scope;
  ImagickImage *image = ObjectWrap::Unwrap<ImagickImage>(args.This());

  try {
    if(args[0]->IsString())
    {
      image->image_.read(to_string(args[0]));
    }
    else if(Buffer::HasInstance(args[0]) || ISB(args[0]))
    {
      Magick::Blob b;

      if (ISB(args[0]))
      {
        b = BUNWRAP(args[0]->ToObject())->opaque_;
      }
      else
      {
        b = to_blob(args[0]);
      }

      switch (args.Length())
      {
        case 1:
          image->image_.read(b);
          break;
        case 2:
          if (ISG(args[1]))
          {
            image->image_.read(b, from_obj(args[1]));
          }
          break;
        case 3:
          if (ISG(args[1]))
          {
            Magick::Geometry g(from_obj(args[1]));

            if(args[2]->IsString())
            {
              image->image_.read(b, g, to_string(args[2]));
            }
            else if(args[2]->IsNumber())
            {
              image->image_.read(b, g, args[2]->Uint32Value());
            }
          }
          break;
      }
    }
    else
    {
      return THROW_STRING("Reading from this type not implemented");
    }
      return args.This();
  } catch (Magick::Error &error) {
    return throw_exception(error);
  }
}

Handle<Value>
ImagickImage::writeSync(const Arguments &args)
{
  HandleScope scope;
  ImagickImage *image = ObjectWrap::Unwrap<ImagickImage>(args.This());
  Local<Value> ret;
  ImagickBlob *blob;

  try
  {
    switch (args.Length())
    {
      case 0:
        blob = new ImagickBlob();
        image->image_.write(&blob->opaque_);
        ret = scope.Close(blob->handle_);
        break;
      case 1:
        if (args[0]->IsString())
        {
          image->image_.write(to_string(args[0]));
          ret = args.This();
        }
        else if(ISB(args[0]))
        {
          blob = BUNWRAP(args[0]->ToObject());
          image->image_.write(&blob->opaque_);
        }
        else
        {
          return THROW_STRING("Argument must be either string or blob");
        }
        break;
      case 2:
        ENSURE_INSTANCE(Blob, 0);
        ENSURE(String, 1);
        blob = BUNWRAP(args[0]->ToObject());
        image->image_.write(&blob->opaque_, to_string(args[1]));
        break;
      case 3:
        ENSURE_INSTANCE(Blob, 0);
        ENSURE(String, 1);
        ENSURE(Number, 2);
        blob = BUNWRAP(args[1]->ToObject());
        image->image_.write(&blob->opaque_, to_string(args[1]), args[2]->Uint32Value());
      default:
        return THROW_STRING("Writing to this type not implemented");
        break;
    }
  } catch (Magick::Error &error) {
    return throw_exception(error);
  }

  return ret;
}

Handle<Value>
ImagickImage::rotate(const Arguments &args)
{
  HandleScope scope;
  ImagickImage *image = ObjectWrap::Unwrap<ImagickImage>(args.This());
  ENSURE(Number, 0);

  try
  {
    image->image_.rotate(args[0]->NumberValue());
  } catch (Magick::Error &error) {
    return throw_exception(error);
  }

  return args.This();
}

Handle<Value>
ImagickImage::adaptiveThreshold(const Arguments &args)
{
  HandleScope scope;
  ENSURE(Number, 0);
  ENSURE(Number, 1);
  ImagickImage *image = IUNWRAP(args.This());

  try
  {
    switch (args.Length())
    {
      case 2:
        image->image_.adaptiveThreshold(args[0]->Uint32Value(), args[1]->Uint32Value());
        break;
      case 3:
        ENSURE(Number, 2);
        image->image_.adaptiveThreshold(args[0]->Uint32Value(), args[1]->Uint32Value(), args[2]->Uint32Value());
        break;
      default:
        return THROW_STRING("adaptiveThreshold only takes 2 or 3 arguments");
        break;
    }
  } catch (Magick::Error &error) {
    return throw_exception(error);
  }

  return args.This();
}

void
ImagickImage::Initialize(Handle<Object> target)
{
  HandleScope scope;
  Local<FunctionTemplate> t = FunctionTemplate::New(New);
  t->InstanceTemplate()->SetInternalFieldCount(1);

  /* Void Calls */
  IMAGICK_PROTOTYPE(t, despeckle);
  IMAGICK_PROTOTYPE(t, enhance);
  IMAGICK_PROTOTYPE(t, equalize);
  IMAGICK_PROTOTYPE(t, erase);
  IMAGICK_PROTOTYPE(t, flip);
  IMAGICK_PROTOTYPE(t, flop);
  IMAGICK_PROTOTYPE(t, magnify);
  IMAGICK_PROTOTYPE(t, minify);
  IMAGICK_PROTOTYPE(t, modifyImage);
  IMAGICK_PROTOTYPE(t, normalize);
  IMAGICK_PROTOTYPE(t, trim);

  /* Geometry Calls */
  IMAGICK_PROTOTYPE(t, border);
  IMAGICK_PROTOTYPE(t, chop);
  IMAGICK_PROTOTYPE(t, crop);
  IMAGICK_PROTOTYPE(t, sample);
  IMAGICK_PROTOTYPE(t, scale);
  IMAGICK_PROTOTYPE(t, shave);
  IMAGICK_PROTOTYPE(t, zoom);

  IMAGICK_PROTOTYPE(t, adaptiveThreshold);
  NODE_SET_PROTOTYPE_METHOD(t, "addNoise", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "addNoiseChannel", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "affineTransform", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "annotate", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "blur", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "blurChannel", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "cdl", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "channel", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "charcol", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "colorize", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "comment", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "compare", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "composite", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "contrast", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "convolve", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "cycleColormap", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "display", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "distort", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "draw", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "edge", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "emboss", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "extent", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "floodFillColor", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "floodFillTexture", NotImplemented);
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
  NODE_SET_PROTOTYPE_METHOD(t, "map", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "matteFloodfill", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "medianFilter", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "modulate", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "motionBlur", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "negate", NotImplemented);
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
  IMAGICK_PROTOTYPE(t, rotate);
  NODE_SET_PROTOTYPE_METHOD(t, "segment", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "shade", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "sharpen", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "sharpenChannel", NotImplemented);
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
  NODE_SET_PROTOTYPE_METHOD(t, "unsharpmask", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "unsharpmaskChannel", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "wave", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "write", NotImplemented);
  NODE_SET_PROTOTYPE_METHOD(t, "writeSync", writeSync);
  //NODE_SET_PROTOTYPE_METHOD(t, "", NotImplemented);

  target->Set(String::NewSymbol("image"), t->GetFunction());
}
