#ifndef MEMFS_H
#define MEMFS_H

#include "include/stdint.h"

#define MAXFILENAME 0x0F
#define MAXFILES    0x0F
#define MAXSUBDIR   0x0F
#define FSADDRESS   0x01000000 

#define PERM_R 0x04
#define PERM_W 0x02
#define PERM_X 0x01

typedef struct Directory Directory;

typedef struct
{
    char filename[MAXFILENAME];
    DWORD size;
    DWORD dataOffset;
    BYTE permissions;
} FileHeader;

struct Directory
{
    char name[MAXFILENAME];
    FileHeader files[MAXFILES];

    Directory* subdirs[MAXSUBDIR];
    Directory* parent;

    DWORD nextFreeBlock;
};

typedef struct
{
    Directory root;
    Directory* currentDir;
} FileSystem;

typedef struct
{
    FileHeader* file;
    DWORD pos;
} FILE;


typedef struct
{
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char checksum[8];
    char typeflag;
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
} TargetHeader;

Directory* currentDir;

void init_filesystem();

int make_file
(
    const char* filename,
    const LPBYTE data,
    DWORD size,
    BYTE permissions
);
int read_file(const char* filename, LPBYTE buffer, LPDWORD size);
int del_file(const char* filename);
int find_file(const CHAR* filename);
int file_info(const CHAR* filename);
INT rename_file(const char* oldFilename, const char* newFilename);
VOID show_all_files();
char* get_file_data(CHAR* path);
FileHeader get_file_h(const CHAR* filename);
int read_chunk
(
    const char* filename,
    LPBYTE buffer,
    DWORD bufferSize,
    DWORD offset,
    DWORD bytes
);

int make_folder(const char* foldername);
int del_folder(const char* foldername);
int find_folder(const char* foldername, Directory** dir);
int rename_folder
(
    const char* oldFoldername,
    const char* newFoldername
);
void show_all_folders();
Directory* get_system_dir();

VOID print_curr_dir();
INT can_run(const char* filename);
INT change_dir(const CHAR* foldername);

#endif