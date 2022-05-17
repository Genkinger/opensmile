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


#ifndef OLMEGA_FEAT_H
#define OLMEGA_FEAT_H

typedef struct
{
    int32_t version;
    int32_t block_count;
    int32_t feature_dimensions;
    int32_t block_size;
    int32_t hop_size;
    int32_t sampling_rate;
    
    char    sample_timestamp[16];
    char    system_timestamp[16];

    float   calibration_channel_1;
    float   calibration_channel_2;

    uint8_t android_identifier[16];
    uint8_t bluetooth_transmitter_mac[17];
} olmega_feat_header_t;

typedef struct
{
    olmega_feat_header_t header;
    void *data;
} olmega_feat_t;


olmega_feat_t *olmega_feat_create_from(void* data, size_t size);
void olmega_feat_destroy(olmega_feat_t *olmega_feat);

void olmega_feat_print_header(olmega_feat_t *olmega_feat);

///==================
/// IMPLEMENTATION
///==================
#ifdef OLMEGA_FEAT_IMPLEMENTATION
#include "blob.h"
#include <assert.h>
#include <string.h>

#if defined(_WIN32)
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif


#ifndef olega_feat_malloc(x)
#define olmega_feat_malloc(x) malloc(x)
#endif

#ifndef olmega_feat_free(x)
#define olmega_feat_free(x) free(x)
#endif

#ifndef ASSERT(Condition,Message)
#define ASSERT(Condition,Message) assert((Condition) && (Message));
#endif

olmega_feat_t*
olmega_feat_create_from(void* data, size_t size)
{
    olmega_feat_t *olmega_feat = olmega_feat_malloc(sizeof(olmega_feat_t));
    blob_t* blob = blob_create_from(data,size);
    olmega_feat->header.version = ntohl(blob_unpack_int32_t(blob));
    ASSERT(olmega_feat->header.version == 4, "Versions other than 4 are not yet supported!");
    olmega_feat->header.block_count = ntohl(blob_unpack_int32_t(blob));
    olmega_feat->header.feature_dimensions = ntohl(blob_unpack_int32_t(blob));
    olmega_feat->header.block_size = ntohl(blob_unpack_int32_t(blob));
    olmega_feat->header.hop_size = ntohl(blob_unpack_int32_t(blob));
    olmega_feat->header.sampling_rate = ntohl(blob_unpack_int32_t(blob));
    strncpy(olmega_feat->header.sample_timestamp,blob_unpack_string(blob,16),16);
    strncpy(olmega_feat->header.system_timestamp,blob_unpack_string(blob,16),16);
    float calibration_channel_1 = blob_unpack_float(blob);
    float calibration_channel_2 = blob_unpack_float(blob);
    char tmp[4] = {0}; 
    tmp[0] = ((char*)(&calibration_channel_1))[3];
    tmp[1] = ((char*)(&calibration_channel_1))[2];
    tmp[2] = ((char*)(&calibration_channel_1))[1];
    tmp[3] = ((char*)(&calibration_channel_1))[0];
    olmega_feat->header.calibration_channel_1 = *((float*)tmp);
    tmp[0] = ((char*)(&calibration_channel_2))[3];
    tmp[1] = ((char*)(&calibration_channel_2))[2];
    tmp[2] = ((char*)(&calibration_channel_2))[1];
    tmp[3] = ((char*)(&calibration_channel_2))[0];
    olmega_feat->header.calibration_channel_2 = *((float*)tmp);
    
//Note(Genki): Identifier + Mac are still missing
    
    return olmega_feat;
}

void
olmega_feat_destroy(olmega_feat_t *olmega_feat)
{
    if(olmega_feat->data)
    {
        olmega_feat_free(olmega_feat->data);
    }
    olmega_feat_free(olmega_feat);
}

void olmega_feat_print_header(olmega_feat_t *feat)
{
    printf("version: %d\n",feat->header.version);
    printf("block_count: %d\n",feat->header.block_count);
    printf("feature_dimensions: %d\n",feat->header.feature_dimensions);
    printf("block_size: %d\n",feat->header.block_size);
    printf("hop_size: %d\n",feat->header.hop_size);
    printf("sampling_rate: %d\n",feat->header.sampling_rate);
    printf("sample_timestamp: %.16s\n",feat->header.sample_timestamp);
    printf("system_timestamp: %.16s\n",feat->header.system_timestamp);
    printf("calibration_channel_1: %f\n", feat->header.calibration_channel_1);
    printf("calibration_channel_2: %f\n", feat->header.calibration_channel_2);
    printf("android identifier: TODO\n");
    printf("bluetooth_transmitter_mac: TODO\n");
}


#endif
#endif
