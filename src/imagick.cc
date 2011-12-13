#include <node.h>
#include "command.h"

using namespace v8;
using namespace node;

extern "C" void
init (Handle<Object> target)
{
  HandleScope scope;
  CommandInit(target);
}
