/*
#   clove
#
#   Copyright (C) 2016-2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "../include/filesystem.h"
#include "../include/utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef CLOVE_WINDOWS
#include <direct.h>
#define getcwd _getcwd // apparently getcwd is dreprecated on windows
#include <io.h>
#define access _access
#else
#include <unistd.h>
#endif


static struct {
	const char* os;
	/*
	 * used when use_physfs is 'true' to
	 * determine the mouting point
	 */
	const char* source;
} moduleData;

void filesystem_init(char* argv0, int stats) {
#ifdef CLOVE_MACOSX
	moduleData.os = "osx";
#elif CLOVE_LINUX
	moduleData.os = "linux";
#elif CLOVE_WINDOWS
	moduleData.os = "windows";
#elif CLOVE_ANDROID
	moduleData.os = "android";
#elif CLOVE_WEB
	moduleData.os = "web";
#else
	moduleData.os = "This OS is not supported";
#endif

	#ifdef USE_PHYSFS
	if (!PHYSFS_init(argv0))
		clove_error(PHYSFS_getLastError());
	#endif

}

void filesystem_free()
{
	#ifdef USE_PHYSFS
	if (PHYSFS_isInit())
		PHYSFS_deinit();
	#endif
}

const char* filesystem_getOS() {
	return moduleData.os;
}

const char* filesystem_getSaveDirectory(const char* company, const char* projName) {
	return SDL_GetPrefPath(company, projName);
}

int filesystem_contain(const char* a, const char* b) {
	if(strstr(a,b))
		return 1;
	else
		return 0;
}
/*
 * This functions checks to see if two strings are the same
 * If you pass a number to argument 'l' then clove will
 * dheck for equality untill the point of 'l' value
 */
int filesystem_equals(const char* a, const char* b, int l) {
	if (l > 0)
	{
		if(strncmp(a, b, l) == 0)
			return 1;
	}
	else
	{
		if (strcmp(a, b) == 0)
			return 1;
	}

	return 0;
}

int filesystem_exists(const char* name)
{
	#ifndef USE_PHYSFS
	FILE* file = fopen(name,"r");
	if(!file){
		return 0;
	}

	fclose(file);
	return 1;

	#else
	return PHYSFS_exists(name);
	#endif

}

int filesystem_write(const char* name, const char* data)
{

	#ifdef USE_PHYSFS

	PHYSFS_File* file;
	file = PHYSFS_openWrite(name);
    if(! file) {
		clove_error(PHYSFS_getLastError());
        return -1;
    }

	int write_size = PHYSFS_write(file, data, sizeof(char), strlen(data));
    if (write_size == -1) {
		clove_error(PHYSFS_getLastError());
        return -2;
    }

	PHYSFS_close(file);

	return write_size;

	#else
	FILE* file = fopen(name, "w");
	if(!file){
        clove_error("Couldn't open filename %s\n", name);
		return -1;
	}

	fseek(file,0,SEEK_END);
	long size = ftell(file);
	rewind(file);

    int wrote = fprintf(file, data);
    if (wrote < 0) {
        fclose(file);
        clove_error("Couldn't write to filename %s\n", name);
        return -2;
    }
    fclose(file);

	return size;
	#endif
}

int filesystem_append(const char* name, const char* data) {

	#ifdef USE_PHYSFS

	PHYSFS_File* file;
	file = PHYSFS_openAppend(name);
    if(! file) {
		clove_error(PHYSFS_getLastError());
        return -1;
    }
	int append_size = PHYSFS_write(file, data, sizeof(char), strlen(data));
    if (append_size == -1) {
        PHYSFS_close(file);
		clove_error(PHYSFS_getLastError());
        return -2;
    }
	PHYSFS_close(file);

	return append_size;

	#else

	FILE* file = fopen(name, "a");
    if (!file) {
        clove_error("Couldn't open file %s for appending\n", name);
        return -1;
    }

	fseek(file,0,SEEK_END);
	long size = ftell(file);
	rewind(file);

    int appended = fprintf(file, data);
    if (appended < 0) {
        clove_error("Couldn't write to file %s for appending\n", name);
        fclose(file);
        return -2;
    }
    fclose(file);

	return size;
	#endif

}

const char* filesystem_getCurrentDirectory() {
	//TODO Look into this
#ifndef CLOVE_WEB
	char buffer[1024];
	const char* dir = getcwd(buffer, sizeof(buffer));
	if (dir != NULL)
		return dir;
#endif
	clove_error("Error, Could not get the current directory \n");
	return NULL;
}

/**
 * mode can be:
 * 0, wether or not the file exist
 * 2, write only
 * 4, read only
 * 6, read and write
 * default is 0
 */
bool filesystem_status(const char* file, int mode) {
	//TODO Look into this
#ifndef CLOVE_WEB
	if (access(file, mode) != -1)
        return true;
#endif
    return false;
}

int filesystem_read(char const* filename, char** output) {

	#ifdef USE_PHYSFS

	PHYSFS_File* file;
	file = PHYSFS_openRead(filename);
    if (!file)
        return -1;
	int size = PHYSFS_fileLength(file);

	*output = malloc(size + 1);

	int len_read = PHYSFS_read(file, *output, 1, PHYSFS_fileLength(file));
	(*output)[size] = '\n';

	PHYSFS_close(file);

	return size;

	#else
	FILE* infile = fopen(filename, "r");
	if(!infile) {
		return -1;
	}

	fseek(infile, 0, SEEK_END);
	long size = ftell(infile);
	rewind(infile);

	*output = malloc(size+1);
	fread(*output, size, 1, infile);
	fclose(infile);
	(*output)[size] = 0;

    return size;
	#endif
}

bool filesystem_mkDir(const char* path)
{
	#ifdef USE_PHYSFS

	return PHYSFS_mkdir(path) != 0;

	#else

	clove_error("mkDir feature is supported by enabling physfs.");
	return false;
	#endif
}

bool filesystem_isDir(const char* dir)
{
	#ifdef USE_PHYSFS
	return PHYSFS_isDirectory(dir) != 0;
	#else

	clove_error("isDir feature is supported by enabling physfs.");
	return false;
	#endif
}

void filesystem_setSource(const char* source, const char* dir)
{
	#ifdef USE_PHYSFS
	if (! PHYSFS_mount(source, dir, 1))
	{
		clove_error("couldn't mount file:");
		clove_error(source);

	}
	moduleData.source = source;
	#else
	clove_error("setSource works only with physfs!");
	#endif
}

const char* filesystem_getSource() {
	#ifdef USE_PHYSFS
	return moduleData.source != NULL ? moduleData.source : SDL_GetBasePath();
	#else
	return SDL_GetBasePath();
	#endif
}

bool filesystem_setIdentity(const char* name)
{
	#ifdef USE_PHYSFS
	const char* save_dir = filesystem_getUsrDir();

	if (! PHYSFS_setWriteDir(save_dir))
	{
		clove_error(PHYSFS_getLastError());
		return false;
	}

	if (! filesystem_mkDir(name))
	{
		PHYSFS_setWriteDir(NULL);
		clove_error(PHYSFS_getLastError());
		return false;
	}

	if (! PHYSFS_setWriteDir(name))
	{
		clove_error(PHYSFS_getLastError());
		return false;
	}

	if (! PHYSFS_mount(name, NULL, 0))
	{
		PHYSFS_setWriteDir(NULL);
		clove_error(PHYSFS_getLastError());
		return false;
	}

	return true;
	#else
	return false;
	#endif
}

bool filesystem_mount(const char* path, const char* mountPoint, int appendToPath)
{

	#ifdef USE_PHYSFS
	return PHYSFS_mount(path, mountPoint, appendToPath) != 0;
	#else
	clove_error("mouting feature is supported by enabling physfs.");
	return false;
	#endif
}

bool filesystem_unmount(const char* path)
{
	#ifdef USE_PHYSFS
	const char* getMountPoint = PHYSFS_getMountPoint(path);
	if (!getMountPoint)
	{
		clove_error("no mouting point for:");
		clove_error(path);

		return false;
	}
	return PHYSFS_removeFromSearchPath(path) != 0;
	#else

	clove_error("unmouting feature is supported by enabling physfs.");
	return false;
	#endif
}

char** filesystem_enumerate(const char* path)
{
	#ifdef USE_PHYSFS
	return PHYSFS_enumerateFiles(path);
	#else
	clove_error("enumerate feature is supported by enabling physfs.");
	return NULL;
	#endif
}

const char* filesystem_getUsrDir()
{
	#ifdef USE_PHYSFS
	return PHYSFS_getUserDir();
	#else
	clove_error("getUsrDir feature is supported by enabling physfs.");
	return NULL;
	#endif
}

bool filesystem_remove(const char* name) {
    return remove(name);
}

bool filesystem_rename(const char *old_name, const char *new_name) {
    return rename(old_name, new_name);
}



