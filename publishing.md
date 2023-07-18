# Publishing

The Roam version of beamcoder only supports binary releases for ease of
distribution. Publishing multiple binary distributions of the same package is a
bit challenging on NPM, but in the end we decided to go the
[esbuild](https://github.com/evanw/esbuild) route and have multiple packages
with just platform-specific binaries, and a main package with just the
javascript.

## Publishing a new version

1. Update the version number in the appropriate
   `prebuilds/{platform}/package.json` (TODO: automate this?)
2. In the root of the repository, `npm run make-prebuilt`
3. Change directory to the `prebuilds/{platform}` folder and run `npm publish`.
4. Edit the version number in the main `package.json`
5. Repeat the above steps on all supported platforms (TODO: support
   cross-compilation someday?)
6. `npm publish` in the root of the repository

## Adding a new supported platform

1. Make a new folder in `prebuilds`. The folder should be named according to
   the outputs of `os.platform()` and `os.arch()` as reported by node.
2. Use an existing folder as a template for what files to put inside the
   folder. A `package.json`, `README.md` stub, and `index.js` stub should all
   be in there.
3. Make sure that `npm run build` can produce a `build/Release/beamcoder.node`
   binary that passes all the tests.
4. Follow the previous instructions to publish it.
