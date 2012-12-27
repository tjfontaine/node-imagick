var binding;

try {
  binding = require('./build/Release/imagick');
} catch(e) {
  binding = require('./build/Debug/imagick');
}

module.exports = binding;
