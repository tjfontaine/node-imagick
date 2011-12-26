#include <node.h>
#include <Magick++.h>

void
ConstantsInitialize(v8::Handle<v8::Object> target)
{
  using namespace Magick;

  NODE_DEFINE_CONSTANT(target, UniformNoise);
  NODE_DEFINE_CONSTANT(target, GaussianNoise);
  NODE_DEFINE_CONSTANT(target, MultiplicativeGaussianNoise);
  NODE_DEFINE_CONSTANT(target, ImpulseNoise);
  NODE_DEFINE_CONSTANT(target, LaplacianNoise);
  NODE_DEFINE_CONSTANT(target, PoissonNoise);

  NODE_DEFINE_CONSTANT(target, UndefinedChannel);
  NODE_DEFINE_CONSTANT(target, RedChannel);
  NODE_DEFINE_CONSTANT(target, CyanChannel);
  NODE_DEFINE_CONSTANT(target, GreenChannel);
  NODE_DEFINE_CONSTANT(target, MagentaChannel);
  NODE_DEFINE_CONSTANT(target, BlueChannel);
  NODE_DEFINE_CONSTANT(target, YellowChannel);
  NODE_DEFINE_CONSTANT(target, OpacityChannel);
  NODE_DEFINE_CONSTANT(target, BlackChannel);
  NODE_DEFINE_CONSTANT(target, MatteChannel);
}
