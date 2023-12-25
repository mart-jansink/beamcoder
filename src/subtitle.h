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

#ifndef SUBTITLE_H
#define SUBTITLE_H

#include "node_api.h"
#include "beamcoder_util.h"
#include "subtitle_rect.h"

extern "C" {
  #include <libavcodec/avcodec.h>
}

void subtitleDataFinalizer(napi_env env, void* data, void* hint);

struct subtitleData {
  AVSubtitle subtitle;
  ~subtitleData() {
    // printf("Freeing subtitle with pts = %i\n", subtitle.pts);
    avsubtitle_free(&subtitle);
  }
};

napi_value makeSubtitle(napi_env env, napi_callback_info info);
napi_status fromAVSubtitle(napi_env env, subtitleData* subtitle, napi_value* result);

#endif // SUBTITLE_H
