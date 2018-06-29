/*
#   clove
#
#   Copyright (C) 2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "audio.h"

#include "../audio/audio.h"
#include "../audio/source.h"
#include "../audio/staticsource.h"
#include "../audio/streamsource.h"

#include "../tools/utils.h"

static struct
{
	audio_Type audio_type;
} moduleData;


ar_Value* ar_audio_newSource(ar_State* S, ar_Value* args, ar_Value* env)
{
	char const* filename = ar_eval_string(S, ar_car(args), env);

	ar_Value* type_value = ar_eval(S, ar_nth(args, 1), env);
	const char* type = "static";
	if (ar_type(type_value) == AR_TSTRING)
		type = type_value->u.str.s;

	audio_StreamSource* stream_src = NULL;
	audio_StaticSource* static_src = NULL;

	int err;
	if (strcmp(type, "stream") == 0)
	{
		stream_src = malloc(sizeof(audio_StreamSource));
		err = audio_loadStream(stream_src, filename);
		moduleData.audio_type = audio_type_stream;
	}
	else if (strcmp(type, "static") == 0)
	{
		static_src = malloc(sizeof(audio_StaticSource));
		err = audio_loadStatic(static_src, filename);
		moduleData.audio_type = audio_type_static;
	}

	if (err == -1)
	{
		clove_error("%s%s%s\n", "Could not load sound file: ", filename, "reason: unknow file type");
		return NULL;
	}
	else if (err == 0)
	{
		clove_error("%s%s%s\n", "Could not load sound file: ", filename, "reason: file does not exist");
		return NULL;
	}

	ar_Value* udata;
	if (stream_src != NULL)
		udata = ar_new_udata(S, stream_src);
	else
		udata = ar_new_udata(S, static_src);
	return udata;
}


ar_Value* ar_audio_SourceCommon_play(ar_State* S, ar_Value* args, ar_Value* env)
{
	if (moduleData.audio_type == audio_type_stream)
	{
		audio_StreamSource* source = (audio_StreamSource*)
			ar_eval_udata(S, ar_car(args), env);
		audio_StreamSource_play(source);
	}
	else
	{
		audio_StaticSource* source = (audio_StaticSource*)
			ar_eval_udata(S, ar_car(args), env);
		audio_StaticSource_play(source);
	}
	return NULL;
}


ar_Value* ar_audio_SourceCommon_stop(ar_State* S, ar_Value* args, ar_Value* env)
{
	if (moduleData.audio_type == audio_type_stream)
	{
		audio_StreamSource* source = (audio_StreamSource*)
			ar_eval_udata(S, ar_car(args), env);
		audio_StreamSource_stop(source);
	}
	else
	{
		audio_StaticSource* source = (audio_StaticSource*)
			ar_eval_udata(S, ar_car(args), env);
		audio_StaticSource_stop(source);
	}
	return NULL;
}


ar_Value* ar_audio_SourceCommon_pause(ar_State* S, ar_Value* args, ar_Value* env)
{
	if (moduleData.audio_type == audio_type_stream)
	{
		audio_StreamSource* source = (audio_StreamSource*)
			ar_eval_udata(S, ar_car(args), env);
		audio_StreamSource_pause(source);
	}
	else
	{
		audio_StaticSource* source = (audio_StaticSource*)
			ar_eval_udata(S, ar_car(args), env);
		audio_StaticSource_pause(source);
	}
	return NULL;
}


ar_Value* ar_audio_SourceCommon_resume(ar_State* S, ar_Value* args, ar_Value* env)
{
	if (moduleData.audio_type == audio_type_stream)
	{
		audio_StreamSource* source = (audio_StreamSource*)
			ar_eval_udata(S, ar_car(args), env);
		audio_StreamSource_resume(source);
	}
	else
	{
		audio_StaticSource* source = (audio_StaticSource*)
			ar_eval_udata(S, ar_car(args), env);
		audio_StaticSource_resume(source);
	}
	return NULL;
}


ar_Value* ar_audio_SourceCommon_isStopped(ar_State* S, ar_Value* args, ar_Value* env)
{
	if (moduleData.audio_type == audio_type_stream)
	{
		audio_StreamSource* source = (audio_StreamSource*)
			ar_eval_udata(S, ar_car(args), env);
		return audio_StreamSource_isStopped(source) == true ? S->t : NULL;
	}
	else
	{
		audio_SourceCommon* source = (audio_SourceCommon*)
			ar_eval_udata(S, ar_car(args), env);
		return audio_SourceCommon_isStopped(source) ? S->t : NULL;
	}
	return NULL;
}


ar_Value* ar_audio_SourceCommon_isPlaying(ar_State* S, ar_Value* args, ar_Value* env)
{
	if (moduleData.audio_type == audio_type_stream)
	{
		audio_StreamSource* source = (audio_StreamSource*)
			ar_eval_udata(S, ar_car(args), env);
		return audio_StreamSource_isPlaying(source) == true ? S->t : NULL;
	}
	else
	{
		audio_SourceCommon* source = (audio_SourceCommon*)
			ar_eval_udata(S, ar_car(args), env);
		return audio_SourceCommon_isPlaying(source) ? S->t : NULL;
	}
	return NULL;
}


ar_Value* ar_audio_SourceCommon_isPaused(ar_State* S, ar_Value* args, ar_Value* env)
{
	if (moduleData.audio_type == audio_type_stream)
	{
		audio_StreamSource* source = (audio_StreamSource*)
			ar_eval_udata(S, ar_car(args), env);
		return audio_StreamSource_isPaused(source) == true ? S->t : NULL;
	}
	else
	{
		audio_SourceCommon* source = (audio_SourceCommon*)
			ar_eval_udata(S, ar_car(args), env);
		return audio_SourceCommon_isPaused(source) ? S->t : NULL;
	}
	return NULL;
}


ar_Value* ar_audio_SourceCommon_setVolume(ar_State* S, ar_Value* args, ar_Value* env)
{
	double gain = ar_eval_number(S, ar_nth(args, 1), env);
	if (moduleData.audio_type == audio_type_stream)
	{
		audio_StreamSource* source = (audio_StreamSource*)
			ar_eval_udata(S, ar_car(args), env);
		audio_StreamSource_setVolume(source, gain);
	}
	else
	{
		audio_SourceCommon* source = (audio_SourceCommon*)
			ar_eval_udata(S, ar_car(args), env);
		audio_SourceCommon_setVolume(source, gain);
	}
	return NULL;
}


ar_Value* ar_audio_SourceCommon_getVolume(ar_State* S, ar_Value* args, ar_Value* env)
{
	if (moduleData.audio_type == audio_type_stream)
	{
		audio_StreamSource* source = (audio_StreamSource*)
			ar_eval_udata(S, ar_car(args), env);
		return ar_new_number(S, audio_StreamSource_getVolume(source));
	}
	else
	{
		audio_SourceCommon* source = (audio_SourceCommon*)
			ar_eval_udata(S, ar_car(args), env);
		return ar_new_number(S, audio_SourceCommon_getVolume(source));
	}
	return NULL;
}


ar_Value* ar_audio_SourceCommon_setVolumeGlobal(ar_State* S, ar_Value* args, ar_Value* env)
{
	double volume = ar_eval_number(S, ar_car(args), env);
	if(volume <= 0.0f)
		volume = 0.0f;
	else if (volume >= 1.0f)
		volume = 1.0f;

	audio_setVolume(volume);
	return NULL;
}


ar_Value* ar_audio_SourceCommon_setPitch(ar_State* S, ar_Value* args, ar_Value* env)
{
	double pitch = ar_eval_number(S, ar_nth(args, 1), env);
	if (moduleData.audio_type == audio_type_stream)
	{
		audio_StreamSource* source = (audio_StreamSource*)
			ar_eval_udata(S, ar_car(args), env);
		audio_StreamSource_setPitch(source, pitch);
	}
	else
	{
		audio_SourceCommon* source = (audio_SourceCommon*)
			ar_eval_udata(S, ar_car(args), env);
		audio_SourceCommon_setPitch(source, pitch);
	}
	return NULL;
}


ar_Value* ar_audio_SourceCommon_getPitch(ar_State* S, ar_Value* args, ar_Value* env)
{
	if (moduleData.audio_type == audio_type_stream)
	{
		audio_StreamSource* source = (audio_StreamSource*)
			ar_eval_udata(S, ar_car(args), env);
		return ar_new_number(S, audio_StreamSource_getPitch(source));
	}
	else
	{
		audio_SourceCommon* source = (audio_SourceCommon*)
			ar_eval_udata(S, ar_car(args), env);
		return ar_new_number(S, audio_SourceCommon_getPitch(source));
	}
	return NULL;
}


ar_Value* ar_audio_SourceCommon_setPosition(ar_State* S, ar_Value* args, ar_Value* env)
{
	float x = ar_eval_number(S, ar_nth(args, 1), env);
	float y = ar_eval_number(S, ar_nth(args, 2), env);
	float z = ar_eval_number(S, ar_nth(args, 3), env);

	if (moduleData.audio_type == audio_type_stream)
	{
		audio_StreamSource* source = (audio_StreamSource*)
			ar_eval_udata(S, ar_car(args), env);
		audio_StreamSource_setPosition(source, x, y, z);
	}
	else
	{
		audio_SourceCommon* source = (audio_SourceCommon*)
			ar_eval_udata(S, ar_car(args), env);
		audio_SourceCommon_setPosition(source, x, y, z);
	}
	return NULL;
}


ar_Value* ar_audio_SourceCommon_setVelocity(ar_State* S, ar_Value* args, ar_Value* env)
{
	float x = ar_eval_number(S, ar_nth(args, 1), env);
	float y = ar_eval_number(S, ar_nth(args, 2), env);
	float z = ar_eval_number(S, ar_nth(args, 3), env);

	if (moduleData.audio_type == audio_type_stream)
	{
		audio_StreamSource* source = (audio_StreamSource*)
			ar_eval_udata(S, ar_car(args), env);
		audio_StreamSource_setVelocity(source, x, y, z);
	}
	else
	{
		audio_SourceCommon* source = (audio_SourceCommon*)
			ar_eval_udata(S, ar_car(args), env);
		audio_SourceCommon_setVelocity(source, x, y, z);
	}
	return NULL;
}


ar_Value* ar_audio_SourceCommon_setLooping(ar_State* S, ar_Value* args, ar_Value* env)
{
	if (moduleData.audio_type == audio_type_stream)
	{
		audio_StreamSource* source = (audio_StreamSource*)
			ar_eval_udata(S, ar_car(args), env);
		audio_StreamSource_setLooping(source, ar_eval_boolean(S, ar_nth(args, 1), env));
	}
	else
	{
		audio_SourceCommon* source = (audio_SourceCommon*)
			ar_eval_udata(S, ar_car(args), env);
		audio_SourceCommon_setPitch(source, ar_eval_boolean(S, ar_nth(args, 1), env));
	}
	return NULL;
}


ar_Value* ar_audio_SourceCommon_free(ar_State* S, ar_Value* args, ar_Value* env)
{
	if (moduleData.audio_type == audio_type_stream)
	{
		audio_StreamSource* source = (audio_StreamSource*)
			ar_eval_udata(S, ar_car(args), env);
		audio_StreamSource_free(source);
		free(source);
	}
	else
	{
		audio_SourceCommon* source = (audio_SourceCommon*)
			ar_eval_udata(S, ar_car(args), env);
		audio_SourceCommon_free(source);
		free(source);
	}
	return NULL;
}


void ar_audio_register(ar_State* S)
{
	struct { const char* name; ar_Prim fn; } prims[] =
	{
		{"love:audio-newSource", 			ar_audio_newSource},
		{"love:audio-play",      			ar_audio_SourceCommon_play},
		{"love:audio-stop",      			ar_audio_SourceCommon_stop},
		{"love:audio-resume",      			ar_audio_SourceCommon_resume},
		{"love:audio-pause",      			ar_audio_SourceCommon_pause},
		{"love:audio-isStopped",      		ar_audio_SourceCommon_isStopped},
		{"love:audio-isPlaying",      		ar_audio_SourceCommon_isPlaying},
		{"love:audio-isPaused",      		ar_audio_SourceCommon_isPaused},
		{"love:audio-setVolume",      		ar_audio_SourceCommon_setVolume},
		{"love:audio-getVolume",      		ar_audio_SourceCommon_getVolume},
		{"love:audio-setVolumeGlobal",      ar_audio_SourceCommon_setVolumeGlobal},
		{"love:audio-setPitch",      		ar_audio_SourceCommon_setPitch},
		{"love:audio-getPitch",      		ar_audio_SourceCommon_getPitch},
		{"love:audio-setVelocity",      	ar_audio_SourceCommon_setVelocity},
		{"love:audio-setPosition",      	ar_audio_SourceCommon_setPosition},
		{"love:audio-setLooping",      		ar_audio_SourceCommon_setLooping},
		{"love:audio-gc",      				ar_audio_SourceCommon_free},
		{NULL, NULL}
	};
	for (int i = 0; prims[i].name; i++)
		ar_bind_global(S, prims[i].name, ar_new_prim(S, prims[i].fn));
}


