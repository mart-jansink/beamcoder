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

napi_value setSubtitleFormat(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result;
  napi_valuetype type;
  subtitleData* s;
  uint32_t format;

  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info, &argc, args, nullptr, (void**) &s);
  CHECK_STATUS;

  if (argc < 1) {
    NAPI_THROW_ERROR("Set subtitle format must be provided with a value.");
  }
  status = napi_typeof(env, args[0], &type);
  CHECK_STATUS;
  if (type != napi_number) {
    NAPI_THROW_ERROR("Subtitle format property must be set with a number.");
  }

  status = napi_get_value_uint32(env, args[0], &format);
  CHECK_STATUS;
  s->subtitle.format = format;

  status = napi_get_undefined(env, &result);
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

napi_value setSubtitleStartDisplaytime(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result;
  napi_valuetype type;
  subtitleData* s;

  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info, &argc, args, nullptr, (void**) &s);
  CHECK_STATUS;

  if (argc < 1) {
    NAPI_THROW_ERROR("Set subtitle start display time must be provided with a value.");
  }
  status = napi_typeof(env, args[0], &type);
  CHECK_STATUS;
  if (type != napi_number) {
    NAPI_THROW_ERROR("Subtitle start display time property must be set with a number.");
  }

  status = napi_get_value_uint32(env, args[0], &s->subtitle.start_display_time);
  CHECK_STATUS;

  status = napi_get_undefined(env, &result);
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

napi_value setSubtitleEndDisplaytime(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result;
  napi_valuetype type;
  subtitleData* s;

  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info, &argc, args, nullptr, (void**) &s);
  CHECK_STATUS;

  if (argc < 1) {
    NAPI_THROW_ERROR("Set subtitle end display time must be provided with a value.");
  }
  status = napi_typeof(env, args[0], &type);
  CHECK_STATUS;
  if (type != napi_number) {
    NAPI_THROW_ERROR("Subtitle end display time property must be set with a number.");
  }

  status = napi_get_value_uint32(env, args[0], &s->subtitle.end_display_time);
  CHECK_STATUS;

  status = napi_get_undefined(env, &result);
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

  for ( unsigned i = 0 ; i < s->subtitle.num_rects ; ++i ) {
    subtitleRectData* r = new subtitleRectData;
    r->subtitleRect = s->subtitle.rects[i];
    r->index = i;

    status = fromAVSubtitleRect(env, r, &element);
    CHECK_STATUS;

    status = napi_set_element(env, array, i, element);
    CHECK_STATUS;
  }

  return array;
}

napi_value setSubtitleRects(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value element, global, jsObject, assign, rectangle, result;
  napi_valuetype type;
  bool isArray;
  unsigned numRects;
  subtitleData* s;
  AVSubtitle* subtitle;

  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info, &argc, args, nullptr, (void**) &s);
  CHECK_STATUS;
  subtitle = &s->subtitle;

  if (argc < 1) {
    NAPI_THROW_ERROR("Set subtitle rects must be provided with a value.");
  }
  status = napi_is_array(env, args[0], &isArray);
  CHECK_STATUS;
  if (!isArray) {
    NAPI_THROW_ERROR("Subtitle rects property must be set with an array.");
  }

  status = napi_get_array_length(env, args[0], &numRects);
  CHECK_STATUS;
  for ( unsigned i = 0 ; i < numRects ; ++i ) {
    status = napi_get_element(env, args[0], i, &element);
    CHECK_STATUS;

    status = napi_typeof(env, element, &type);
    CHECK_STATUS;

    if (type != napi_object) {
      NAPI_THROW_ERROR("Subtitle rects can only be created from an options object.");
    }
  }

  for ( unsigned i = 0 ; i < subtitle->num_rects; ++i ) {
    AVSubtitleRect *const rectangle = subtitle->rects[i];

    av_freep(&rectangle->data[0]);
    av_freep(&rectangle->data[1]);
    av_freep(&rectangle->data[2]);
    av_freep(&rectangle->data[3]);
    av_freep(&rectangle->text);
    av_freep(&rectangle->ass);
  }
  av_freep(&subtitle->rects);

  subtitle->num_rects = numRects;
  subtitle->rects = static_cast<AVSubtitleRect**>(av_calloc(numRects, sizeof(*subtitle->rects)));
  if (!subtitle->rects) {
    NAPI_THROW_ERROR(avErrorMsg("Problem allocating subtitle rect: ", AVERROR(ENOMEM)));
  }
  for ( unsigned i = 0 ; i < numRects ; ++i ) {
    subtitle->rects[i] = static_cast<AVSubtitleRect*>(av_mallocz(sizeof(*subtitle->rects[i])));
    if (!subtitle->rects[i]) {
      NAPI_THROW_ERROR(avErrorMsg("Problem allocating subtitle rect: ", AVERROR(ENOMEM)));
    }
  }

  status = napi_get_global(env, &global);
  CHECK_STATUS;
  status = napi_get_named_property(env, global, "Object", &jsObject);
  CHECK_STATUS;
  status = napi_get_named_property(env, jsObject, "assign", &assign);
  CHECK_STATUS;

  for ( unsigned i = 0 ; i < numRects ; ++i ) {
    status = napi_get_element(env, args[0], i, &element);
    CHECK_STATUS;

    subtitleRectData* r = new subtitleRectData;
    r->subtitleRect = subtitle->rects[i];
    r->index = i;

    status = fromAVSubtitleRect(env, r, &rectangle);
    CHECK_STATUS;

    const napi_value fargs[] = { rectangle, element };
    status = napi_call_function(env, rectangle, assign, 2, fargs, &rectangle);
    CHECK_STATUS;
  }

  status = napi_get_undefined(env, &result);
  CHECK_STATUS;
  return result;
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
  }

  return result;
}

napi_value setSubtitlePTS(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result;
  napi_valuetype type;
  subtitleData* s;

  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info, &argc, args, nullptr, (void**) &s);
  CHECK_STATUS;

  if (argc < 1) {
    NAPI_THROW_ERROR("Set subtitle PTS must be provided with a value.");
  }
  status = napi_typeof(env, args[0], &type);
  CHECK_STATUS;
  if ((type == napi_null) || (type == napi_undefined)) {
    s->subtitle.pts = AV_NOPTS_VALUE;
    goto done;
  }
  if (type != napi_number) {
    NAPI_THROW_ERROR("Subtitle PTS property must be set with a number.");
  }

  status = napi_get_value_int64(env, args[0], &s->subtitle.pts);
  CHECK_STATUS;

done:
  status = napi_get_undefined(env, &result);
  CHECK_STATUS;
  return result;
}

napi_value makeSubtitle(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result, global, jsObject, assign, jsJSON, jsParse;
  napi_valuetype type;
  bool isArray;
  subtitleData* s = new subtitleData;
  memset(&s->subtitle, 0, sizeof(s->subtitle));

  status = napi_get_global(env, &global);
  CHECK_STATUS;

  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
  CHECK_STATUS;
  if (argc > 1) {
    NAPI_THROW_ERROR("Subtitle may be created with zero or one options object argument.");
  }
  if (argc == 1) {
    status = napi_typeof(env, args[0], &type);
    CHECK_STATUS;
    if (type == napi_string) {
      status = napi_get_named_property(env, global, "JSON", &jsJSON);
      CHECK_STATUS;
      status =  napi_get_named_property(env, jsJSON, "parse", &jsParse);
      CHECK_STATUS;
      const napi_value pargs[] = { args[0] };
      status = napi_call_function(env, args[0], jsParse, 1, pargs, &args[0]);
      CHECK_STATUS;
      status = napi_typeof(env, args[0], &type);
      CHECK_STATUS;
    }
    status = napi_is_array(env, args[0], &isArray);
    CHECK_STATUS;
    if (isArray || (type != napi_object)) {
      NAPI_THROW_ERROR("Cannot create a subtitle without an options object.");
    }
  }

  status = fromAVSubtitle(env, s, &result);
  CHECK_STATUS;

  if (argc == 1) {
    status = napi_get_named_property(env, global, "Object", &jsObject);
    CHECK_STATUS;
    status = napi_get_named_property(env, jsObject, "assign", &assign);
    CHECK_STATUS;
    const napi_value fargs[] = { result, args[0] };
    status = napi_call_function(env, result, assign, 2, fargs, &result);
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
    { "format", nullptr, nullptr, getSubtitleFormat, setSubtitleFormat, nullptr,
      (napi_property_attributes) (napi_writable | napi_enumerable), s },
    { "start_display_time", nullptr, nullptr, getSubtitleStartDisplaytime, setSubtitleStartDisplaytime, nullptr,
      (napi_property_attributes) (napi_writable | napi_enumerable), s },
    { "end_display_time", nullptr, nullptr, getSubtitleEndDisplaytime, setSubtitleEndDisplaytime, nullptr,
      (napi_property_attributes) (napi_writable | napi_enumerable), s },
    { "rects", nullptr, nullptr, getSubtitleRects, setSubtitleRects, nullptr,
      (napi_property_attributes) (napi_writable | napi_enumerable), s },
    { "pts", nullptr, nullptr, getSubtitlePTS, setSubtitlePTS, nullptr,
      (napi_property_attributes) (napi_writable | napi_enumerable), s },
    { "_subtitle", nullptr, nullptr, nullptr, nullptr, extSubtitle, napi_default, nullptr }
  };
  status = napi_define_properties(env, jsSubtitle, 7, desc);
  PASS_STATUS;

  *result = jsSubtitle;
  return napi_ok;
}

void subtitleDataFinalizer(napi_env env, void* data, void* hint) {
  subtitleData* s = (subtitleData*) data;
  // printf("Freeing subtitle data for pts = %i\n", s->subtitle.pts);
  delete s;
}
