#include <node.h>
#include <Magick++.h>

void
ConstantsInitialize(v8::Handle<v8::Object> target)
{
  NODE_DEFINE_CONSTANT(target, Magick::UniformNoise);
  NODE_DEFINE_CONSTANT(target, Magick::GaussianNoise);
  NODE_DEFINE_CONSTANT(target, Magick::MultiplicativeGaussianNoise);
  NODE_DEFINE_CONSTANT(target, Magick::ImpulseNoise);
  NODE_DEFINE_CONSTANT(target, Magick::LaplacianNoise);
  NODE_DEFINE_CONSTANT(target, Magick::PoissonNoise);
}
