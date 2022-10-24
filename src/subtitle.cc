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

#include "subtitle.h"

napi_value getSubtitleTypeName(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result;

  status = napi_create_string_utf8(env, "Subtitle", NAPI_AUTO_LENGTH, &result);
  CHECK_STATUS;

  return result;
}

napi_value getSubtitleFormat(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result;
  subtitleData* s;

  status = napi_get_cb_info(env, info, 0, nullptr, nullptr, (void**) &s);
  CHECK_STATUS;

  status = napi_create_uint32(env, s->subtitle.format, &result);
  CHECK_STATUS;

  return result;
}

napi_value getSubtitleStartDisplaytime(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result;
  subtitleData* s;

  status = napi_get_cb_info(env, info, 0, nullptr, nullptr, (void**) &s);
  CHECK_STATUS;

  status = napi_create_uint32(env, s->subtitle.start_display_time, &result);
  CHECK_STATUS;

  return result;
}

napi_value getSubtitleEndDisplaytime(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result;
  subtitleData* s;

  status = napi_get_cb_info(env, info, 0, nullptr, nullptr, (void**) &s);
  CHECK_STATUS;

  status = napi_create_uint32(env, s->subtitle.end_display_time, &result);
  CHECK_STATUS;

  return result;
}

napi_value getSubtitleNumRects(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result;
  subtitleData* s;

  status = napi_get_cb_info(env, info, 0, nullptr, nullptr, (void**) &s);
  CHECK_STATUS;

  status = napi_create_uint32(env, s->subtitle.num_rects, &result);
  CHECK_STATUS;

  return result;
}

napi_value getSubtitleRects(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value array, element;
  subtitleData* s;

  status = napi_get_cb_info(env, info, 0, nullptr, nullptr, (void**) &s);
  CHECK_STATUS;

  status = napi_create_array_with_length(env, s->subtitle.num_rects, &array);
  CHECK_STATUS;

  for ( unsigned index = 0 ; index < s->subtitle.num_rects ; ++index ) {
    subtitleRectData* r = new subtitleRectData;
    r->subtitleRect = *s->subtitle.rects[ index ];
    r->index = index;

    status = fromAVSubtitleRect(env, r, &element);
    CHECK_STATUS;

    status = napi_set_element(env, array, index, element);
    CHECK_STATUS;
  }

  return array;
}

napi_value getSubtitlePTS(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result;
  subtitleData* s;

  status = napi_get_cb_info(env, info, 0, nullptr, nullptr, (void**) &s);
  CHECK_STATUS;

  if (s->subtitle.pts == AV_NOPTS_VALUE) {
    status = napi_get_null(env, &result);
    CHECK_STATUS;
  } else {
    status = napi_create_int64(env, s->subtitle.pts, &result);
    CHECK_STATUS;
  }

  return result;
}

napi_status fromAVSubtitle(napi_env env, subtitleData* s, napi_value* result) {
  napi_status status;
  napi_value jsSubtitle, extSubtitle;

  status = napi_create_object(env, &jsSubtitle);
  PASS_STATUS;
  status = napi_create_external(env, s, subtitleDataFinalizer, nullptr, &extSubtitle);
  PASS_STATUS;

  napi_property_descriptor desc[] = {
    { "type", nullptr, nullptr, getSubtitleTypeName, nop, nullptr, napi_enumerable, nullptr },
    { "format", nullptr, nullptr, getSubtitleFormat, nullptr, nullptr,
      (napi_property_attributes) (napi_enumerable), s },
    { "start_display_time", nullptr, nullptr, getSubtitleStartDisplaytime, nullptr, nullptr,
      (napi_property_attributes) (napi_enumerable), s },
    { "end_display_time", nullptr, nullptr, getSubtitleEndDisplaytime, nullptr, nullptr,
      (napi_property_attributes) (napi_enumerable), s },
    { "num_rects", nullptr, nullptr, getSubtitleNumRects, nullptr, nullptr,
      (napi_property_attributes) (napi_enumerable), s },
    { "rects", nullptr, nullptr, getSubtitleRects, nullptr, nullptr,
      (napi_property_attributes) (napi_enumerable), s },
    { "pts", nullptr, nullptr, getSubtitlePTS, nullptr, nullptr,
      (napi_property_attributes) (napi_enumerable), s },
    { "_subtitle", nullptr, nullptr, nullptr, nullptr, extSubtitle, napi_default, nullptr }
  };
  status = napi_define_properties(env, jsSubtitle, 8, desc);
  PASS_STATUS;

  *result = jsSubtitle;
  return napi_ok;
}

void subtitleDataFinalizer(napi_env env, void* data, void* hint) {
  subtitleData* s = (subtitleData*) data;
  // printf("Freeing subtitle data for pts = %i\n", s->subtitle->pts);
  delete s;
}
