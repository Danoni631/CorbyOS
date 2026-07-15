/*
    CODED BY Danoni631, on 07/10/2026
    inspired on NovaOS File System definitions

    This code have file functions, folder functions and
    other.
*/
#include "alloc.h"
#include "mem.h"
#include "graphics.h"
#include "elf.h"

#include "fs.h"

FileSystem* fs = (FileSystem*) FSADDRESS;

void init_filesystem()
{
    fs = (FileSystem*) FSADDRESS;

    memset(fs, 0, sizeof(FileSystem));

    strncpy(fs->root.name, "", MAXFILENAME);

    fs->root.parent = NULL;

    fs->root.nextFreeBlock = (FSADDRESS + sizeof(FileSystem) + 3) & ~3;

    currentDir = &fs->root;
}

int make_file
(
    const char* filename,
    const LPBYTE data,
    DWORD size,
    BYTE permissions
)
{
    if (size == 0 || strlen(filename) >= MAXFILENAME)
        return -1;

    int fileIndex = -1;

    for (int i = 0; i < MAXFILES; i++)
    {
        if
        (
            currentDir->files[i].filename[0] ==
            '\0'
        )
        {
            fileIndex = i;
            break;
        }
    }

    if (fileIndex == -1) return -2;

    DWORD dataOffset = (currentDir->nextFreeBlock + 3) & ~3;

    currentDir->nextFreeBlock = dataOffset + size;

    strncpy
    (
        currentDir->files[fileIndex].filename,
        filename, MAXFILENAME
    );
    currentDir->files[fileIndex].size = size;
    currentDir->files[fileIndex].dataOffset = dataOffset;
    currentDir->files[fileIndex].permissions = permissions;

    memcpy((void*)dataOffset, data, size);

    return 0;
}

int read_file(const char* filename, LPBYTE buffer, LPDWORD size)
{
    for (int i = 0; i < MAXFILES; i++) 
    {
        if
        (
            strncmp
            (
                currentDir->files[i].filename,
                filename,
                MAXFILENAME
            ) == 0
        ) 
        {
            if (!(currentDir->files[i].permissions & PERM_R))
            {
                debug("Permission Denied!\n", 0x01); 
                return -1;
            }

            *size = currentDir->files[i].size;

            if (buffer != NULL)
            {
                LPBYTE fileData =
                (LPBYTE) currentDir->files[i].dataOffset;
                memcpy(buffer, fileData, *size);
            }

            return 0x00;
        }
    }

    print("\n\nInvalid File :(", 0xFFFF0000);

    return -1;
}

int del_file(const char* filename)
{
    for (int i = 0; i < MAXFILES; i++) 
    {
        if
        (
            strncmp
            (
                currentDir->files[i].filename,
                filename,
                MAXFILENAME
            ) == 0
        ) 
        {
            memset(currentDir->files[i].filename, 0x00, MAXFILENAME);

            currentDir->files[i].size = 0x00000000;
            currentDir->files[i].dataOffset = 0x00000000;
            currentDir->files[i].permissions = 0x00;

            return 0x00;
        }
    }

    print("\n\nInvalid File :(", 0xFFFF0000);

    return -1;     
}

int file_info(const CHAR* filename)
{
    print("\n", 0x00);

    int index = find_file(filename);

    if (index == -1)
    {
        return -1;
    }

    FileHeader* file = &currentDir->files[index];

    for (int i = 0; i < MAXFILES; i++) 
    {
        if
        (
            strncmp
            (
                currentDir->files[i].filename,
                filename,
                MAXFILENAME
            ) == 0
        ) 
        {
            print("\nFile name: ", 0xFF00FF00);
            print(currentDir->files[i].filename, 0xFFFFFFFF);

            print("\nFile size: ", 0xFF00FF00);
            printint(currentDir->files[i].size, 0xFFFFFFFF);

            print("\nData offset: ", 0xFF00FF00);
            printint(currentDir->files[i].dataOffset, 0xFFFFFFFF);

            print("\nAll permissions: ", 0xFF00FF00);

            print((file->permissions & PERM_R)  ? "R" : "-", 0xFFFFFFFF);
            print((file->permissions & PERM_W) ? "W" : "-", 0xFFFFFFFF);
            print((file->permissions & PERM_X)  ? "X" : "-", 0xFFFFFFFF);
            
            return 0x00;
        }
    }

    print("\n\nInvalid File :(", 0xFFFF0000);

    return -1;
}

int find_file(const CHAR* filename)
{
    for (int i = 0; i < MAXFILES; i++) 
    {
        if
        (
            strncmp
            (
                currentDir->files[i].filename,
                filename,
                MAXFILENAME
            ) == 0
        ) 
        {
            return i;
        }
    }

    print("\n\nInvalid File!", 0xFFFF0000);

    return -1;
}

INT rename_file(const char* oldFilename, const char* newFilename)
{
    int fileIndex = find_file(oldFilename);

    if (fileIndex == -1) 
    {
        return -1;
    }

    if (strlen(newFilename) >= MAXFILENAME) 
    {
        return -2; 
    }

    strncpy
    (
        currentDir->files[fileIndex].filename,
        newFilename,
        MAXFILENAME
    );

    return 0x00;
}

VOID show_all_files()
{
    print("\n\n", 0x00);

    for (int i = 0; i < MAXFILES; i++)
    {
        if (currentDir->files[i].filename[0] != '\0')
        {
            print(currentDir->files[i].filename, 0xF0FFFFFF);
            print(" ", 0x00);
        }
    }

    for (int i = 0; i < MAXSUBDIR; i++)
    {
        if (currentDir->subdirs[i] != NULL)
        {
            print(currentDir->subdirs[i]->name, 0xFFFFFFF0);
            print(" ", 0x00);
        }
    }
}

char* get_file_data(CHAR* path)
{
    CHAR* last = path;

    for (int i = 0; path[i] != '\0'; i++)
    {
        if (path[i] == '/')
        {
            last = &path[i + 1];
        }
    }

    return last;
}

FileHeader* get_file_p(const CHAR* filename)
{
    Directory* dir = currentDir;

    for (int i = 0; i < MAXFILES; i++)
    {
        if (dir->files[i].filename[0] == 0)
            continue;

        if (strcmp(dir->files[i].filename, filename) == 0)
        {
            return &dir->files[i];
        }
    }

    return NULL;
}

int read_chunk
(
    const char* filename,
    LPBYTE buffer,
    DWORD bufferSize,
    DWORD offset,
    DWORD bytes
)
{
    FileHeader* file = get_file_p(filename);

    if (file == NULL) return -1;

    if (offset >= file->size) return -2;

    if (offset + bytes > file->size) bytes = file->size - offset;

    BYTE* src = (BYTE*)(file->dataOffset + offset);

    memcpy(buffer, src, bytes);

    return bytes;
}

int make_folder(const char* foldername)
{
    if
    (
        strlen(foldername)
        >= MAXFILENAME || foldername[0]
        == '\0'
    ) return -1;

    for (int i = 0; i < MAXSUBDIR; i++)
    {
        if (currentDir->subdirs[i] == NULL)
        {
            DWORD addr = (currentDir->nextFreeBlock + 3) & ~3;

            Directory* new_folder = (Directory*) addr;

            currentDir->nextFreeBlock = addr + sizeof(Directory);

            memset(new_folder, 0, sizeof(Directory));

            strncpy
            (
                new_folder->name,
                foldername,
                MAXFILENAME - 1
            );

            new_folder->name[MAXFILENAME - 1] = '\0';

            new_folder->parent = currentDir;
            new_folder->nextFreeBlock =
            (DWORD)new_folder + sizeof(Directory);

            currentDir->subdirs[i] = new_folder;

            return 0;
        }
    }

    return -2;
}

int del_folder(const char* foldername)
{
    for (int i = 0; i < MAXSUBDIR; i++)
    {
        if
        (
            currentDir->subdirs[i] != 0x00 &&
            strncmp
            (
                currentDir->subdirs[i]->name,
                foldername, MAXFILENAME
            ) == 0x00
        )
        {
            Directory* dir = currentDir->subdirs[i];

            for (int j = 0; j < MAXFILES; j++)
            {
                if (dir->files[j].filename[0] != '\0')
                {
                    return -1;
                }
            }

            for (int j = 0; j < MAXSUBDIR; j++)
            {
                if (dir->subdirs[j] != NULL)
                {
                    return -1;
                }
            }

            currentDir->subdirs[i] = NULL;

            return 0x00;
        }
    }

    return -2;
}

void show_all_folders()
{
    for (int i = 0; i < MAXSUBDIR; i++)
    {
        if (fs->currentDir->subdirs[i] != NULL)
        {
            print(fs->currentDir->subdirs[i]->name, 0xFFFF0FFF);
            print(" ", 0x00);
        }
    }
}

Directory* get_system_dir()
{
    return currentDir;
}

VOID print_curr_dir()
{
    print("LOCAL_DISK", 0xF0FFFFFF);
    print("/", 0xFFFFFFFF);
    print("CORBY_CORE", 0x0000FF00);
    print("/", 0xFFFFFFFF);

    char path[256] = "";
    Directory* temp = currentDir;

    while (temp != NULL)
    {
        char buffer[MAXFILENAME + 2];
        buffer[0] = '/';
        strncpy(buffer + 1, temp->name, MAXFILENAME);
        buffer[MAXFILENAME + 1] = '\0';

        char tempPath[256];
        strcpy(tempPath, path);
        strcpy(path, buffer);
        strcat(path, tempPath);

        temp = temp->parent;
    }

    for (int i = 0; i < 256; i++)
    {
        path[i] = path[i + 1];
    }
    
    print(path,  0xFFFFFFFF);
    print("/ ", 0xFFFFFFFF);
}

INT can_run(const char* filename)
{
    int index = find_file(filename);

    if (index == -1)
    {
        return -1;
    }

    if (currentDir->files[index].permissions & PERM_X)
    {
        return 1;
    }

    return 0;
}

INT change_dir(const CHAR* foldername)
{
    if (strcmp(foldername, "..") == 0x00)
    {
        if (currentDir->parent != 0x00)
        {
            currentDir = currentDir->parent;

            return 0x00;
        }

        return -1;
    }

    for (int i = 0; i < MAXSUBDIR; i++)
    {
        if
        (
            currentDir->subdirs[i] != 0x00 &&
            strncmp
            (
                currentDir->subdirs[i]->name,
                foldername,
                MAXFILENAME
            ) == 0x00
        )
        {
            currentDir = currentDir->subdirs[i];
            return 0x00;
        }
    }

    return -2;
}