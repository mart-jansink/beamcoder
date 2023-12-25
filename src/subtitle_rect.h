/*
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef SUBTITLE_RECT_H
#define SUBTITLE_RECT_H

#include "node_api.h"
#include "beamcoder_util.h"

extern "C" {
  #include <libavcodec/avcodec.h>
}

void subtitleRectDataFinalizer(napi_env env, void* data, void* hint);

struct subtitleRectData {
  AVSubtitleRect* subtitleRect = nullptr;
  unsigned index;
  int32_t extSize = 0;
  ~subtitleRectData() {
    // printf("Freeing subtitle rect at index = %i\n", index);
  }
};

napi_status fromAVSubtitleRect(napi_env env, subtitleRectData* subtitleRect, napi_value* result);

#endif // SUBTITLE_RECT_H
