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

#include <assert.h>
#define ASSERT(Condition,Message) assert(Condition && Message)

#define OLMEGA_IMPLEMENTATION
#include "olmega.h"

uint8_t *ReadEntireFile(const char* FilePath, size_t *OutputSize)
{
    uint8_t *Data = NULL;    
    FILE *FileDescriptor = fopen(FilePath, "rb");
    fseek(FileDescriptor,0,SEEK_END);
    *OutputSize = ftell(FileDescriptor);
    rewind(FileDescriptor);
    Data = malloc(*OutputSize);
    ASSERT(Data,"Allocation failure!");
    fread(Data,*OutputSize,1,FileDescriptor);
    fclose(FileDescriptor);

    return(Data);
}

void WriteStringToFile(const char* FilePath, char* Data, size_t Size)
{
    FILE *FileDescriptor = fopen(FilePath, "wb");
    fwrite(Data,Size,1,FileDescriptor);
    fclose(FileDescriptor);
}

int32_t
main(int32_t Count, char **Arguments)
{

    ASSERT(Count == 3, "Invalid number of arguments!");
    
    size_t Size = 0;
    size_t CSVSize = 0;
    uint8_t *Data = NULL;
    olmega_feat Feat = {};
    char* CSVString = NULL;
    
    Data = ReadEntireFile(Arguments[1],&Size);
    Feat = olmega_feat_create(Data,Size);    
    CSVString = olmega_csv_string_create(Feat,&CSVSize);
    
    WriteStringToFile(Arguments[2],CSVString,CSVSize);

    olmega_csv_string_destroy(CSVString);
    olmega_feat_destroy(Feat);
    free(Data);
    
    return(0);
}
