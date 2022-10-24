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

#include "subtitle_rect.h"

napi_value getSubtitleRectX(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result;
  subtitleRectData* r;

  status = napi_get_cb_info(env, info, 0, nullptr, nullptr, (void**) &r);
  CHECK_STATUS;

  if (r->subtitleRect.type != AVSubtitleType::SUBTITLE_BITMAP) {
    status = napi_get_null(env, &result);
    CHECK_STATUS;

    return result;
  }

  status = napi_create_int32(env, r->subtitleRect.x, &result);
  CHECK_STATUS;

  return result;
}

napi_value getSubtitleRectY(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result;
  subtitleRectData* r;

  status = napi_get_cb_info(env, info, 0, nullptr, nullptr, (void**) &r);
  CHECK_STATUS;

  if (r->subtitleRect.type != AVSubtitleType::SUBTITLE_BITMAP) {
    status = napi_get_null(env, &result);
    CHECK_STATUS;

    return result;
  }

  status = napi_create_int32(env, r->subtitleRect.y, &result);
  CHECK_STATUS;

  return result;
}

napi_value getSubtitleRectW(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result;
  subtitleRectData* r;

  status = napi_get_cb_info(env, info, 0, nullptr, nullptr, (void**) &r);
  CHECK_STATUS;

  if (r->subtitleRect.type != AVSubtitleType::SUBTITLE_BITMAP) {
    status = napi_get_null(env, &result);
    CHECK_STATUS;

    return result;
  }

  status = napi_create_int32(env, r->subtitleRect.w, &result);
  CHECK_STATUS;

  return result;
}

napi_value getSubtitleRectH(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result;
  subtitleRectData* r;

  status = napi_get_cb_info(env, info, 0, nullptr, nullptr, (void**) &r);
  CHECK_STATUS;

  if (r->subtitleRect.type != AVSubtitleType::SUBTITLE_BITMAP) {
    status = napi_get_null(env, &result);
    CHECK_STATUS;

    return result;
  }

  status = napi_create_int32(env, r->subtitleRect.h, &result);
  CHECK_STATUS;

  return result;
}

napi_value getSubtitleRectNbColors(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result;
  subtitleRectData* r;

  status = napi_get_cb_info(env, info, 0, nullptr, nullptr, (void**) &r);
  CHECK_STATUS;

  if (r->subtitleRect.type != AVSubtitleType::SUBTITLE_BITMAP) {
    status = napi_get_null(env, &result);
    CHECK_STATUS;

    return result;
  }

  status = napi_create_int32(env, r->subtitleRect.nb_colors, &result);
  CHECK_STATUS;

  return result;
}

napi_value getSubtitleRectType(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result;
  subtitleRectData* r;
  const char* type;

  status = napi_get_cb_info(env, info, 0, nullptr, nullptr, (void**) &r);
  CHECK_STATUS;

  switch (r->subtitleRect.type) {
    case AVSubtitleType::SUBTITLE_NONE:
      type = "none";
      break;
    case AVSubtitleType::SUBTITLE_BITMAP:
      type = "bitmap";
      break;
    case AVSubtitleType::SUBTITLE_TEXT:
      type = "text";
      break;
    case AVSubtitleType::SUBTITLE_ASS:
      type = "ass";
      break;
    default:
      type = nullptr;
      break;
  }

  if (type == nullptr) {
    status = napi_get_null(env, &result);
  } else {
    status = napi_create_string_utf8(env, type, NAPI_AUTO_LENGTH, &result);
  }
  CHECK_STATUS;
  return result;
}

napi_value getSubtitleRectData(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result, array, element;
  subtitleRectData* r;
  uint8_t* data;
  AVBufferRef* ref;
  size_t size;
  int curElem;

  status = napi_get_cb_info(env, info, 0, nullptr, nullptr, (void**) &r);
  CHECK_STATUS;

  if (r->subtitleRect.type != AVSubtitleType::SUBTITLE_BITMAP) {
    status = napi_get_null(env, &result);
    CHECK_STATUS;

    return result;
  }

  status = napi_create_array_with_length(env, 2, &array);
  CHECK_STATUS;

  size = (r->subtitleRect.h * r->subtitleRect.linesize[0]);
  status = napi_create_external_buffer(env, size, r->subtitleRect.data[0], nullptr, nullptr, &element);
  CHECK_STATUS;

  status = napi_set_element(env, array, 0, element);
  CHECK_STATUS;

  size = (r->subtitleRect.nb_colors * 4);
  status = napi_create_external_buffer(env, size, r->subtitleRect.data[1], nullptr, nullptr, &element);
  CHECK_STATUS;

  status = napi_set_element(env, array, 1, element);
  CHECK_STATUS;

  return array;
}

napi_value getSubtitleRectLinesize(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result;
  subtitleRectData* r;

  status = napi_get_cb_info(env, info, 0, nullptr, nullptr, (void**) &r);
  CHECK_STATUS;

  if (r->subtitleRect.type != AVSubtitleType::SUBTITLE_BITMAP) {
    status = napi_get_null(env, &result);
    CHECK_STATUS;

    return result;
  }

  status = napi_create_int32(env, r->subtitleRect.linesize[0], &result);
  CHECK_STATUS;

  return result;
}

napi_value getSubtitleRectText(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result;
  subtitleRectData* r;

  status = napi_get_cb_info(env, info, 0, nullptr, nullptr, (void**) &r);
  CHECK_STATUS;

  if (r->subtitleRect.type != AVSubtitleType::SUBTITLE_TEXT) {
    status = napi_get_null(env, &result);
    CHECK_STATUS;

    return result;
  }

  status = napi_create_string_utf8(env, r->subtitleRect.text, NAPI_AUTO_LENGTH, &result);
  CHECK_STATUS;

  return result;
}

napi_value getSubtitleRectAss(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result;
  subtitleRectData* r;

  status = napi_get_cb_info(env, info, 0, nullptr, nullptr, (void**) &r);
  CHECK_STATUS;

  if (r->subtitleRect.type != AVSubtitleType::SUBTITLE_ASS) {
    status = napi_get_null(env, &result);
    CHECK_STATUS;

    return result;
  }

  status = napi_create_string_utf8(env, r->subtitleRect.ass, NAPI_AUTO_LENGTH, &result);
  CHECK_STATUS;

  return result;
}

napi_value getSubtitleRectFlags(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result;
  subtitleRectData* r;

  status = napi_get_cb_info(env, info, 0, nullptr, nullptr, (void**) &r);
  CHECK_STATUS;

  status = napi_create_int32(env, r->subtitleRect.flags, &result);
  CHECK_STATUS;

  return result;
}

napi_status fromAVSubtitleRect(napi_env env, subtitleRectData* r, napi_value* result) {
  napi_status status;
  napi_value jsSubtitleRect, extSubtitleRect;
  int64_t externalMemory;

  status = napi_create_object(env, &jsSubtitleRect);
  PASS_STATUS;
  status = napi_create_external(env, r, subtitleRectDataFinalizer, nullptr, &extSubtitleRect);
  PASS_STATUS;

  napi_property_descriptor desc[] = {
    { "x", nullptr, nullptr, getSubtitleRectX, nullptr, nullptr,
      (napi_property_attributes) (napi_enumerable), r },
    { "y", nullptr, nullptr, getSubtitleRectY, nullptr, nullptr,
      (napi_property_attributes) (napi_enumerable), r },
    { "w", nullptr, nullptr, getSubtitleRectW, nullptr, nullptr,
      (napi_property_attributes) (napi_enumerable), r },
    { "h", nullptr, nullptr, getSubtitleRectH, nullptr, nullptr,
      (napi_property_attributes) (napi_enumerable), r },
    { "nb_colors", nullptr, nullptr, getSubtitleRectNbColors, nullptr, nullptr,
      (napi_property_attributes) (napi_enumerable), r },
    { "data", nullptr, nullptr, getSubtitleRectData, nullptr, nullptr,
      (napi_property_attributes) (napi_enumerable), r },
    { "linesize", nullptr, nullptr, getSubtitleRectLinesize, nullptr, nullptr,
      (napi_property_attributes) (napi_enumerable), r },
    { "type", nullptr, nullptr, getSubtitleRectType, nullptr, nullptr,
      (napi_property_attributes) (napi_enumerable), r },
    { "text", nullptr, nullptr, getSubtitleRectText, nullptr, nullptr,
      (napi_property_attributes) (napi_enumerable), r },
    // 10
    { "ass", nullptr, nullptr, getSubtitleRectAss, nullptr, nullptr,
      (napi_property_attributes) (napi_enumerable), r },
    { "flags", nullptr, nullptr, getSubtitleRectFlags, nullptr, nullptr,
      (napi_property_attributes) (napi_enumerable), r },
    { "_subtitleRect", nullptr, nullptr, nullptr, nullptr, extSubtitleRect, napi_default, nullptr }
  };
  status = napi_define_properties(env, jsSubtitleRect, 12, desc);
  PASS_STATUS;

  r->extSize += (r->subtitleRect.h * r->subtitleRect.linesize[0]);
  r->extSize += (r->subtitleRect.nb_colors * 4);
  status = napi_adjust_external_memory(env, r->extSize, &externalMemory);
  PASS_STATUS;

  *result = jsSubtitleRect;
  return napi_ok;
}

void subtitleRectDataFinalizer(napi_env env, void* data, void* hint) {
  napi_status status;
  int64_t externalMemory;
  subtitleRectData* r = (subtitleRectData*) data;
  // printf("Freeing subtitle rect data for index = %i\n", r->index);
  status = napi_adjust_external_memory(env, -r->extSize, &externalMemory);
  if (status != napi_ok) {
    printf("DEBUG: Failed to adjust external memory downwards on subtitle delete.\n");
  }
  delete r;
}
