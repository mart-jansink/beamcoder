#!/bin/bash

# Run
# `brew fetch --force --bottle-tag=arm64_ventura ffmpeg`
# to download a build of ffmpeg for an alternate arch, extract it to a
# directory and edit bindings.gyp to point to that directory, and then run
# `CC="clang -arch arm64" CXX="clang++ -arch arm64" npm_config_arch=arm64 npx node-gyp rebuild`
# to build that binary.
# Then, run this script on the resulting binary to correct the dynamic links.

if [ "$#" -ne 1 ] || ! [ -f "$1" ]
then
  echo "Usage: $0 <file to patch>"
  exit 1
fi

var="HOMEBREW_PREFIX"
prefix="/opt/homebrew"

libs="$(otool -L $1 | grep -o "@@$var@@/.*dylib")"
for lib in $libs
do
  new_lib="$(echo $lib | sed "s#@@$var@@#$prefix#")"
  install_name_tool -change "$lib" "$new_lib" $1
done

codesign -f -s - "$1"
