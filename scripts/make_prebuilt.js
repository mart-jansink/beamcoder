'use strict';

// TODO(jack): error handling, cross-platform builds

const child_process = require('child_process');
const fs = require('fs');
const os = require('os');

// Run a fresh build
child_process.execSync('npm run build');

// Copy the resulting binary to the output folder
fs.copyFileSync(
  'build/Release/beamcoder.node',
  `prebuilds/${os.platform()}-${os.arch()}/beamcoder.node`,
);
