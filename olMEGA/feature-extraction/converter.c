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
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#define ASSERT(Condition,Message) assert(Condition && Message)

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


int32_t main(int32_t Count, char **Arguments)
{
    ASSERT(Count == 2, "Invalid number of arguments!");
    size_t Size = 0;
    char *CSVString = NULL;

    CSVString = ReadEntireFile(Arguments[1],&Size);
    for(size_t Index = 0; Index < Size; Index++)
    {
        if(CSVString[Index] == ',')
        {
            CSVString[Index] = ';';
            continue;
        }
        if(CSVString[Index] == '.')
        {
            CSVString[Index] = ',';
            continue;
        }
        
    }
    
    WriteStringToFile(Arguments[1],CSVString,Size);
    free(CSVString);
    return(0);
}

