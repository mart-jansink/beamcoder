{
  "targets": [{
    "target_name" : "beamcoder",
    "sources" : [ "src/beamcoder.cc", "src/beamcoder_util.cc",
                  "src/log.cc" ,
                  "src/governor.cc", "src/demux.cc",
                  "src/decode.cc", "src/filter.cc",
                  "src/encode.cc", "src/mux.cc",
                  "src/packet.cc", "src/frame.cc",
                  "src/codec_par.cc", "src/format.cc",
                  "src/codec.cc", "src/hwcontext.cc"],
    "conditions": [
      ['OS!="win"', {
        "defines": [
          "__STDC_CONSTANT_MACROS"
        ],
        "cflags_cc!": [
          "-fno-rtti",
          "-fno-exceptions"
        ],
        "cflags_cc": [
          "-std=c++11",
          "-fexceptions"
        ]
      }],
      ['OS!="win" and OS!="linux"', {
        "link_settings": {
          "libraries": [
            "-lavcodec",
            "-lavdevice",
            "-lavfilter",
            "-lavformat",
            "-lavutil",
            "-lpostproc",
            "-lswresample",
            "-lswscale"
          ]
        }
      }],
      ['OS=="win"', {
        "configurations": {
          "Release": {
            "msvs_settings": {
              "VCCLCompilerTool": {
                "RuntimeTypeInfo": "true"
              }
            }
          }
        },
        "include_dirs" : [
          "ffmpeg/ffmpeg-6.x-win64-shared/include"
        ],
        "libraries": [
          "-l../ffmpeg/ffmpeg-6.x-win64-shared/lib/avcodec",
          "-l../ffmpeg/ffmpeg-6.x-win64-shared/lib/avdevice",
          "-l../ffmpeg/ffmpeg-6.x-win64-shared/lib/avfilter",
          "-l../ffmpeg/ffmpeg-6.x-win64-shared/lib/avformat",
          "-l../ffmpeg/ffmpeg-6.x-win64-shared/lib/avutil",
          "-l../ffmpeg/ffmpeg-6.x-win64-shared/lib/postproc",
          "-l../ffmpeg/ffmpeg-6.x-win64-shared/lib/swresample",
          "-l../ffmpeg/ffmpeg-6.x-win64-shared/lib/swscale"
        ],
        "copies": [
            {
              "destination": "build/Release/",
              "files": [
                "ffmpeg/ffmpeg-6.x-win64-shared/bin/avcodec-60.dll",
                "ffmpeg/ffmpeg-6.x-win64-shared/bin/avdevice-60.dll",
                "ffmpeg/ffmpeg-6.x-win64-shared/bin/avfilter-9.dll",
                "ffmpeg/ffmpeg-6.x-win64-shared/bin/avformat-60.dll",
                "ffmpeg/ffmpeg-6.x-win64-shared/bin/avutil-58.dll",
                "ffmpeg/ffmpeg-6.x-win64-shared/bin/postproc-57.dll",
                "ffmpeg/ffmpeg-6.x-win64-shared/bin/swresample-4.dll",
                "ffmpeg/ffmpeg-6.x-win64-shared/bin/swscale-7.dll"
              ]
            }
          ]
    }],
    ['OS=="linux"', {
      "libraries": [
        "<!(pkg-config --libs libavcodec)",
        "<!(pkg-config --libs libavdevice)",
        "<!(pkg-config --libs libavfilter)",
        "<!(pkg-config --libs libavformat)",
        "<!(pkg-config --libs libavutil)",
        "<!(pkg-config --libs libpostproc)",
        "<!(pkg-config --libs libswresample)",
        "<!(pkg-config --libs libswscale)"
      ]
    }],
    ['OS=="mac"', {
      "include_dirs" : [
        "/usr/local/Cellar/ffmpeg/6.0/include"
      ],
      "library_dirs": [
        "/usr/local/Cellar/ffmpeg/6.0/lib",
      ]
    }],
  ]
}]
}
