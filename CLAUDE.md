# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project summary

Aerostat Beam Coder ("beamcoder") is a Node.js native addon that provides N-API bindings directly to FFmpeg's libav* libraries (demuxing, decoding, filtering, encoding, muxing). It is not a wrapper that shells out to the `ffmpeg` CLI (contrast with `fluent-ffmpeg`) — all work happens in-process via async native calls that resolve as JS promises.

## Build / install

- `npm install` runs two steps: `preinstall` (`node install_ffmpeg.js`, fetches/prepares FFmpeg per platform) then `install` (`node-gyp rebuild`, compiles the native addon from `src/*.cc` against libav*).
- There is no standalone `npm run build` script. To recompile native code after editing `src/`, run `node-gyp rebuild` (or re-run `npm install`).
- Platform-specific linking, defined in `binding.gyp`:
  - **macOS**: hardcodes a Homebrew Cellar include/lib path (currently `/opt/homebrew/Cellar/ffmpeg/6.0/...`) — this may need bumping when Homebrew's ffmpeg version changes.
  - **Linux**: resolves libav* via `pkg-config`, requires FFmpeg dev packages installed system-wide.
  - **Windows**: links against a bundled `ffmpeg/ffmpeg-5.x-win64-shared` directory tree and copies DLLs into `build/Release/`.

## Test

- Run all tests: `npm test` (runs `tape test/*.js`).
- Run a single test file: `npx tape test/<name>.js` (e.g. `npx tape test/decoderSpec.js`).
- Test files live in `test/`, one per subsystem: `codecParamsSpec`, `decoderSpec`, `demuxerSpec`, `encoderSpec`, `filtererSpec`, `formatSpec`, `frameSpec`, `introspectionSpec`, `muxerSpec`, `packetSpec`.
- There is no subtitle test spec yet, despite subtitle native code existing (see Known gaps below) — be aware of this rather than assuming subtitle behavior is covered.

## Lint

- `npm run lint` — eslint over `**/*.js`.
- `npm run lint-fix` — auto-fix.
- Config (`.eslintrc.js`): 2-space indent, single quotes, mandatory semicolons, `prefer-arrow-callback`.

## CI

CircleCI (`.circleci/config.yml`), not GitHub Actions. The build job installs, lints (JUnit output), then runs `npm test` piped through `tap-xunit`, on a Docker image with Node 16 + FFmpeg 5.0 baked in.

## Architecture

Two layers:

1. **Native layer** (`src/*.cc` / `*.h`) — plain N-API (`node_api.h`, not NAN or node-addon-api). Async operations (decode/encode/demux/mux/filter) use the `napi_create_async_work` execute/complete pattern to surface as JS promises. One `.cc`/`.h` pair per FFmpeg concept:
   - `demux.cc` — read-side `AVFormatContext`.
   - `mux.cc` — write-side `AVFormatContext`. Both `demux.cc` and `mux.cc` use `governor.cc`/`adaptor.h` for streaming I/O when input/output isn't a plain file (custom AVIO read/write callbacks feeding JS Readable/Writable streams).
   - `decode.cc` / `encode.cc` — dispatch across video/audio/subtitle codec types.
   - `filter.cc` — libavfilter graph construction/execution.
   - `format.cc` — `AVFormatContext`/stream introspection and property accessors (176KB, mostly repetitive getters/setters — grep for the field you need rather than reading the whole file).
   - `codec.cc` — `AVCodecContext` property bindings (236KB, largest file, same grep-don't-read advice applies).
   - `codec_par.cc` — `AVCodecParameters` bindings.
   - `packet.cc` / `frame.cc` — `AVPacket`/`AVFrame` wrappers.
   - `subtitle.cc` / `subtitle_rect.cc` — `AVSubtitle`/`AVSubtitleRect` wrappers (newest addition).
   - `hwcontext.cc` — hardware-acceleration device/frame contexts.
   - `log.cc` — FFmpeg log-level bridging.
   - `beamcoder.cc` — module `Init()`, registers all top-level exported functions.
   - `beamcoder_util.cc` / `.h` — shared macros/helpers (`DECLARE_NAPI_METHOD`, `CHECK_STATUS`, JS↔native value conversion) used across the other files.

2. **JS layer** — `index.js` is thin: loads the compiled `.node` binary via `bindings`, installs a segfault handler, and re-exports native factory functions (`demuxer`, `decoder`, `encoder`, `filterer`, `muxer`, `packet`, `frame`, `subtitle`, introspection helpers) largely unchanged — there's no JS-side OOP reshaping of native objects. `beamstreams.js` is the one real JS-side abstraction layer on top of the raw promise API: Node.js `Readable`/`Writable`/`Transform` wrappers (`demuxerStream`, `muxerStream`) and stream-topology helpers (`serialBalancer`/`parallelBalancer`/`teeBalancer`, `makeSources`, `makeStreams`).

Canonical pipeline shape (see README for full examples): `demuxer(url)` → `dm.read()` packets → `decoder({demuxer, stream_index})` → `dec.decode(packet)` → optional `filterer(...)` → `encoder({...})` → `enc.encode(frame)` → `muxer({...})` → `mux.writeHeader()`/`writeFrame()`/`writeTrailer()`. Everything is promise-based.

Typed via `index.d.ts` + `types/*.d.ts` (one file per major class: CodecPar, Packet, Frame, Stream, Codec, CodecContext, FormatContext, Demuxer, Decoder, Filter, Encoder, Muxer, HWContext, Beamstreams, PrivClass).

### Known gap: subtitle support is not fully wired up

Recent work (native `subtitle.cc`/`subtitle_rect.cc`, subtitle handling added to `decode.cc`/`encode.cc`) has no corresponding `types/Subtitle.d.ts`, no `test/*Spec.js` coverage, and isn't documented in the README. Don't assume subtitle features are complete end-to-end just because the native code exists — check each layer.

### Pattern for adding a new decode/encode feature

Derived from how subtitle support was added (commits `067d293` → `c518705`):
1. Add/extend a dedicated `<feature>.cc`/`.h` pair mirroring `frame.cc`/`packet.cc`.
2. Extend the shared `decode.cc`/`encode.cc` dispatch for the new media/codec type.
3. Register any new top-level factory function in `beamcoder.cc`'s `Init()` property table.
4. Add the new source file(s) to `binding.gyp`'s `sources` list.
5. Update `index.d.ts`/`types/` and add a `test/*Spec.js` file — the subtitle work itself skipped these two steps, which is why the gap above exists. Don't repeat that omission.
