#include <node.h>

#include "command.h"
#include "image.h"
#include "geometry.h"

using namespace v8;
using namespace node;

extern "C" void
init (Handle<Object> target)
{
  HandleScope scope;
  CommandInit(target);
  ImagickImage::Initialize(target);
  ImagickGeometry::Initialize(target);
}
