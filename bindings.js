try {
  // Use locally-built version if possible
  module.exports = require('./build/Release/beamcoder.node');
} catch(_) {
  // Use a remote version if there is no local version
  const os = require('os');
  module.exports = (`@roamhq/beamcoder-${os.platform()}-${os.arch()}`);
}
// TODO(jack): more informative error messages on failure
