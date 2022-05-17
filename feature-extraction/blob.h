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


#ifndef BLOB_H
#define BLOB_H

#define _define_blob_unpack_func(type) type blob_unpack_##type(blob_t *blob)

typedef struct
{
    void* data;
    size_t size;
    size_t position;
} blob_t;

blob_t*   blob_create_from(void *data, size_t size);
void      blob_destroy(blob_t* blob);

_define_blob_unpack_func(uint8_t);
_define_blob_unpack_func(uint16_t);
_define_blob_unpack_func(uint32_t);
_define_blob_unpack_func(uint64_t);

_define_blob_unpack_func(int8_t);
_define_blob_unpack_func(int16_t);
_define_blob_unpack_func(int32_t);
_define_blob_unpack_func(int64_t);

_define_blob_unpack_func(float);
_define_blob_unpack_func(double);

char* blob_unpack_string(blob_t* blob, size_t length);


///===================
/// IMPLEMENTATION
///===================
#ifdef BLOB_IMPLEMENTATION

#ifndef blob_malloc(x)
#define blob_malloc(x) malloc(x)
#endif

#ifndef blob_free(x)
#define blob_free(x) free(x)
#endif

#define blob_peek(type,blob) (*((type*)(blob->data + blob->position)))
#define blob_update_position(type,blob)         \
    do{                                         \
        blob->position += sizeof(type);         \
    }while(0)


#undef _define_blob_unpack_func(type)
#define _define_blob_unpack_func(type)          \
    type blob_unpack_##type(blob_t *blob)       \
    {                                           \
        type value = blob_peek(type,blob);      \
        blob_update_position(type,blob);        \
        return value;                           \
    }                                           
    
blob_t*
blob_create_from(void *data, size_t size)
{
    blob_t *blob = blob_malloc(sizeof(blob_t));
    blob->data = data;
    blob->size = size;
    blob->position = 0;
    return blob;
}

void
blob_destroy(blob_t* blob)
{
    free(blob);
}

_define_blob_unpack_func(uint8_t)
_define_blob_unpack_func(uint16_t)
_define_blob_unpack_func(uint32_t)
_define_blob_unpack_func(uint64_t)

_define_blob_unpack_func(int8_t)
_define_blob_unpack_func(int16_t)
_define_blob_unpack_func(int32_t)
_define_blob_unpack_func(int64_t)

_define_blob_unpack_func(float)
_define_blob_unpack_func(double)

char*
blob_unpack_string(blob_t* blob, size_t length){
    char* value = (char*)(blob->data + blob->position);
    blob->position += sizeof(char) * length;
    return value;
}

#endif
#endif
