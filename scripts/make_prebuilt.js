'use strict';

// TODO(jack): error handling, cross-platform builds

const child_process = require('child_process');
const fs = require('fs');
const os = require('os');

// Run a fresh build
child_process.execSync('npm run build', { stdio: 'inherit' });

// Copy the resulting binary to the output folder
const input_folder = 'build/Release';
const output_folder = `prebuilds/${os.platform()}-${os.arch()}`;
fs.copyFileSync(
  `${input_folder}/beamcoder.node`,
  `${output_folder}/beamcoder.node`,
);

// If on windows, copy all DLLs to the output folder too
if (os.platform() === 'win32') {
  [
    'avcodec-60.dll',
    'avdevice-60.dll',
    'avfilter-9.dll',
    'avformat-60.dll',
    'avutil-58.dll',
    'postproc-57.dll',
    'swresample-4.dll',
    'swscale-7.dll',
  ].forEach(file => {
    fs.copyFileSync(
      `${input_folder}/${file}`,
      `${output_folder}/${file}`,
    );
  });
}
