///================================
/*
Copyright © 2022 <L. Genkinger (genkinger@protonmail.com)>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
///================================



#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#include <assert.h>

#define ASSERT(Condition,Message) (assert((Condition) && (Message)))

#define BLOB_IMPLEMENTATION
#include "blob.h"

#define OLMEGA_FEAT_IMPLEMENTATION
#include "olmega_feat.h"




typedef struct
{
    size_t Size;
    uint8_t *Data;
} load_file_result;

load_file_result
LoadFile(const char* Filepath){
    load_file_result Result = {0};
    FILE *File = fopen(Filepath,"rb");
    ASSERT(File,"Failed to open File");
    fseek(File,0,SEEK_END);
    Result.Size = ftell(File);
    rewind(File);
    Result.Data = malloc(Result.Size);
    ASSERT(Result.Size == fread(Result.Data,1,Result.Size,File),"Failed to read file");
    fclose(File);
    return(Result);
}

void
FreeFile(load_file_result Result)
{
    free(Result.Data);
}

int32_t
main(int32_t Count, char** Arguments)
{
    load_file_result Result = LoadFile(Arguments[1]);
    olmega_feat_t *Feat = olmega_feat_create_from(Result.Data,Result.Size);
    olmega_feat_print_header(Feat);
    olmega_feat_destroy(Feat);
    FreeFile(Result);
    return(0);
}

