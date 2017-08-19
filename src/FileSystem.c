#include "FileSystem.h"

#include "Data/FileList.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

int FileSystem_fileExists(const char *filename)
{
	FILE *fp = fopen(filename, "rb");
	if (!fp) {
		return 0;
	}
	fclose(fp);
	return 1;
}

int FileSystem_readFilePartIntoBuffer(const char *filename, void *buffer, int length, int offsetInFile)
{
	const char *casedFile = FileSystem_getCaseSensitiveFile(filename);
	if (!casedFile) {
		// Try again in 555 directory
		char path[64] = "555/";
		strncpy(&path[4], filename, 60);
		path[63] = 0;
		casedFile = FileSystem_getCaseSensitiveFile(filename);
	}
	if (!casedFile) {
		return 0;
	}
	int bytesRead = 0;
	FILE *fp = fopen(casedFile, "rb");
	if (fp) {
		int seekResult = fseek(fp, offsetInFile, SEEK_SET);
		if (seekResult == 0) {
			bytesRead = fread(buffer, 1, length, fp);
		}
		fclose(fp);
	}
	return bytesRead;
}

int FileSystem_readPartialRecordDataIntoBuffer(const char *filename, void *buffer, int recordReadSize, int recordSkipSize, int numRecords, int startOffset)
{
	int bytesRead = 0;

	FILE *fp = fopen(filename, "rb");
	if (!fp) {
		char filename555[100] = "555/";
		strcpy(&filename555[4], filename);
		fp = fopen(filename555, "rb");
		if (!fp) {
			return 0;
		}
	}

	if (fseek(fp, startOffset, SEEK_SET) == 0) {
		for (int i = 0; i < numRecords; i++) {
			bytesRead += fread(buffer, 1, recordReadSize, fp);
			fseek(fp, recordSkipSize, SEEK_CUR);
			buffer = &((char*)buffer)[recordReadSize];
		}
	}
	fclose(fp);
	return bytesRead;
}


int FileSystem_readFileIntoBuffer(const char *filename, void *buffer, int maxSize)
{
	const char *casedFile = FileSystem_getCaseSensitiveFile(filename);
	if (!casedFile) {
		return 0;
	}
	FILE *fp = fopen(casedFile, "rb");
	if (!fp) {
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	if (size > maxSize) {
		size = maxSize;
	}
	fseek(fp, 0, SEEK_SET);
	int read = fread(buffer, 1, size, fp);
	fclose(fp);
	return read > 0 ? 1 : 0;
}

int FileSystem_getFileSize(const char *filename)
{
	const char *casedFile = FileSystem_getCaseSensitiveFile(filename);
	if (!casedFile) {
		return 0;
	}
	FILE *fp = fopen(casedFile, "rb");
	if (!fp) {
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fclose(fp);
	return size;
}

int FileSystem_writeBufferToFile(const char *filename, const void *buffer, int size)
{
	const char *casedFile = FileSystem_getCaseSensitiveFile(filename);
	if (!casedFile) {
		casedFile = filename;
	}
	FILE *fp = fopen(casedFile, "wb");
	if (!fp) {
		return 0;
	}
	fwrite(buffer, 1, size, fp);
	fclose(fp);
	return 1;
}

void FileSystem_changeExtension(char *filename, const char *newExtension)
{
	char c;
	do {
		c = *filename;
		filename++;
	} while (c != '.' && c);
	if (c == '.') {
		filename[0] = newExtension[0];
		filename[1] = newExtension[1];
		filename[2] = newExtension[2];
		filename[3] = 0;
	}
}

void FileSystem_appendExtension(char *filename, const char *extension)
{
	char c;
	do {
		c = *filename;
		filename++;
	} while (c);
	filename--;
	filename[0] = '.';
	filename[1] = extension[0];
	filename[2] = extension[1];
	filename[3] = extension[2];
	filename[4] = 0;
}

void FileSystem_removeExtension(char *filename)
{
	char c;
	do {
		c = *filename;
		filename++;
	} while (c != '.' && c);
	if (c == '.') {
		filename--;
		*filename = 0;
	}
}

static int compareLower(const void *va, const void *vb)
{
	const char *a = (const char*)va;
	const char *b = (const char*)vb;
	while (*a && *b) {
		int aa = tolower(*a);
		int bb = tolower(*b);
		if (aa != bb) {
			return aa - bb;
		}
		++a;
		++b;
	}
	if (*a) {
		return -1;
	}
	if (*b) {
		return 1;
	}
	return 0;
}

int FileSystem_hasExtension(const char *filename, const char *extension)
{
	char c;
	do {
		c = *filename;
		filename++;
	} while (c != '.' && c);
	return compareLower(filename, extension) == 0;
}

void FileSystem_findFilesWithExtension(const char *extension)
{
	Data_FileList.numFiles = 0;
	for (int i = 0; i < FILE_MAX; i++) {
		Data_FileList.files[i][0] = 0;
	}

	DIR *d = opendir(".");
	if (!d) {
		return;
	}
	struct dirent *entry;
	while ((entry = readdir(d)) && Data_FileList.numFiles < FILE_MAX) {
		if (FileSystem_hasExtension(entry->d_name, extension)) {
			strncpy(Data_FileList.files[Data_FileList.numFiles], entry->d_name, FILENAME_LENGTH - 1);
			Data_FileList.files[Data_FileList.numFiles][FILENAME_LENGTH - 1] = 0;
			++Data_FileList.numFiles;
		}
	}
	closedir(d);
	qsort(Data_FileList.files, Data_FileList.numFiles, FILENAME_LENGTH, compareLower);
}

static int makeCaseInsensitive(const char *dir, char *filename)
{
	DIR *d = opendir(dir);
	if (!d) {
		return 0;
	}
	struct dirent *entry;
	while ((entry = readdir(d))) {
		if (strcasecmp(entry->d_name, filename) == 0) {
			strcpy(filename, entry->d_name);
			closedir(d);
			return 1;
		}
	}
	closedir(d);
	return 0;
}

static void moveLeft(char *str)
{
	while (*str) {
		str[0] = str[1];
		str++;
	}
	*str = 0;
}

const char* FileSystem_getCaseSensitiveFile(const char* filename)
{
	static char insensitiveFilename[200];
	
	strncpy(insensitiveFilename, filename, 200);
	if (FileSystem_fileExists(filename)) {
		return insensitiveFilename;
	}

	char *slash = strchr(insensitiveFilename, '/');
	if (!slash) {
		slash = strchr(insensitiveFilename, '\\');
	}
	if (slash) {
		*slash = 0;
		if (makeCaseInsensitive(".", insensitiveFilename)) {
			char *path = slash + 1;
			if (*path == '\\') {
				// double backslash: move everything to the left
				moveLeft(path);
			}
			if (makeCaseInsensitive(insensitiveFilename, path)) {
				*slash = '/';
				return insensitiveFilename;
			}
		}
	} else {
		if (makeCaseInsensitive(".", insensitiveFilename)) {
			return insensitiveFilename;
		}
	}
	return 0;
}
