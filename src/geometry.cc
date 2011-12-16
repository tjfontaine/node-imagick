#include "geometry.h"

#include "util.h"

#define GEOMETRY_WIDTH String::NewSymbol("width")
#define GEOMETRY_HEIGHT String::NewSymbol("height")
#define GEOMETRY_XOFF String::NewSymbol("xOff")
#define GEOMETRY_YOFF String::NewSymbol("yOff")
#define GEOMETRY_XNEG String::NewSymbol("xNegative")
#define GEOMETRY_YNEG String::NewSymbol("yNegativeh")
#define GEOMETRY_PERCENT String::NewSymbol("percent")
#define GEOMETRY_ASPECT String::NewSymbol("aspect")
#define GEOMETRY_GREATER String::NewSymbol("greater")
#define GEOMETRY_LESS String::NewSymbol("less")
#define GEOMETRY_ISVALID String::NewSymbol("isValid")

using namespace v8;
using namespace node;

static Handle<Value>
NumberGetter(Local<String> property, const AccessorInfo& info)
{
  HandleScope scope;
  ImagickGeometry *geom = ObjectWrap::Unwrap<ImagickGeometry>(info.This());
  size_t value;

  if(GEOMETRY_WIDTH == property)
    value = geom->geo_.width();
  else if (GEOMETRY_HEIGHT == property)
    value = geom->geo_.height();
  else if (GEOMETRY_XOFF == property)
    value = geom->geo_.xOff();
  else if (GEOMETRY_YOFF == property)
    value = geom->geo_.yOff();
  else if (GEOMETRY_XNEG == property)
    value = geom->geo_.xNegative();
  else if (GEOMETRY_YNEG == property)
    value = geom->geo_.yNegative();

  return scope.Close(Number::New(value));
}

static void
NumberSetter (Local<String> property, Local<Value> value, const AccessorInfo& info)
{
  ImagickGeometry *geom = ObjectWrap::Unwrap<ImagickGeometry>(info.This());
  size_t new_value = value->Uint32Value();

  if(GEOMETRY_WIDTH == property)
    geom->geo_.width(new_value);
  else if (GEOMETRY_HEIGHT == property)
    geom->geo_.height(new_value);
  else if (GEOMETRY_XOFF == property)
    geom->geo_.xOff(new_value);
  else if (GEOMETRY_YOFF == property)
    geom->geo_.yOff(new_value);
  else if (GEOMETRY_XNEG == property)
    geom->geo_.xNegative(new_value);
  else if (GEOMETRY_YNEG == property)
    geom->geo_.yNegative(new_value);
}

static Handle<Value>
BoolGetter(Local<String> property, const AccessorInfo& info)
{
  HandleScope scope;
  ImagickGeometry *geom = ObjectWrap::Unwrap<ImagickGeometry>(info.This());
  bool value;

  if(GEOMETRY_PERCENT == property)
    value = geom->geo_.percent();
  else if (GEOMETRY_ASPECT == property)
    value = geom->geo_.aspect();
  else if (GEOMETRY_GREATER == property)
    value = geom->geo_.greater();
  else if (GEOMETRY_LESS == property)
    value = geom->geo_.less();
  else if (GEOMETRY_ISVALID == property)
    value = geom->geo_.isValid();

  return scope.Close(Boolean::New(value));
}

static void
BoolSetter (Local<String> property, Local<Value> value, const AccessorInfo& info)
{
  ImagickGeometry *geom = ObjectWrap::Unwrap<ImagickGeometry>(info.This());
  bool new_value = value->ToBoolean()->Value();

  if(GEOMETRY_PERCENT == property)
    geom->geo_.percent(new_value);
  else if (GEOMETRY_ASPECT == property)
    geom->geo_.aspect(new_value);
  else if (GEOMETRY_GREATER == property)
    geom->geo_.greater(new_value);
  else if (GEOMETRY_LESS == property)
    geom->geo_.less(new_value);
  else if (GEOMETRY_ISVALID == property)
    geom->geo_.isValid(new_value);
}

Handle<Value>
ImagickGeometry::New(const Arguments &args)
{
  HandleScope scope;

  ImagickGeometry *geom = new ImagickGeometry();

  if (args[0]->IsString())
  {
    geom->geo_ = to_string(args[0]);
  }
  else if (args[0]->IsNumber())
  {
    geom->geo_ = Magick::Geometry(
      args[0]->Uint32Value(),
      args[1]->Uint32Value(),
      args[2]->Uint32Value(),
      args[3]->Uint32Value(),
      args[4]->ToBoolean()->Value(),
      args[4]->ToBoolean()->Value()
    );
  }

  geom->Wrap(args.This());

  return args.This();
}

Handle<Value>
ImagickGeometry::toString(const Arguments &args)
{
  HandleScope scope;
  ImagickGeometry *geom = ObjectWrap::Unwrap<ImagickGeometry>(args.This());
  Local<Value> s(String::New(((std::string)geom->geo_).c_str()));
  return scope.Close(s);
}

void
ImagickGeometry::Initialize(Handle<Object> target)
{
  HandleScope scope;
  Local<FunctionTemplate> t = FunctionTemplate::New(New);
  t->InstanceTemplate()->SetInternalFieldCount(1);

  IMAGICK_PROTOTYPE(t, toString);

  t->PrototypeTemplate()->SetAccessor(GEOMETRY_WIDTH, NumberGetter, NumberSetter);
  t->PrototypeTemplate()->SetAccessor(GEOMETRY_HEIGHT, NumberGetter, NumberSetter);
  t->PrototypeTemplate()->SetAccessor(GEOMETRY_XOFF, NumberGetter, NumberSetter);
  t->PrototypeTemplate()->SetAccessor(GEOMETRY_YOFF, NumberGetter, NumberSetter);
  t->PrototypeTemplate()->SetAccessor(GEOMETRY_XNEG, NumberGetter, NumberSetter);
  t->PrototypeTemplate()->SetAccessor(GEOMETRY_YNEG, NumberGetter, NumberSetter);
  t->PrototypeTemplate()->SetAccessor(GEOMETRY_PERCENT, NumberGetter, NumberSetter);

  t->PrototypeTemplate()->SetAccessor(GEOMETRY_ASPECT, BoolGetter, BoolSetter);
  t->PrototypeTemplate()->SetAccessor(GEOMETRY_GREATER, BoolGetter, BoolSetter);
  t->PrototypeTemplate()->SetAccessor(GEOMETRY_LESS, BoolGetter, BoolSetter);
  t->PrototypeTemplate()->SetAccessor(GEOMETRY_ISVALID, BoolGetter, BoolSetter);

  target->Set(String::NewSymbol("geometry"), t->GetFunction());
}
