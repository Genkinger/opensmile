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

#ifndef OLMEGA_H
#define OLMEGA_H

//TODO(L): Write simple usage instructions
//// USAGE ////
/*
  
  Do this:
  #define OLMEGA_IMPLEMENTATION
  before you include this file in *one* C or C++ file to create the implementation.
  
 */








//// INTERFACE ////
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(OLMEGA_STATIC)
#define OLMEGADEF static
#else
#define OLMEGADEF extern
#endif

//TODO(L): Find out what these should actually be
#define OLMEGA_CSV_RMS_HEADINGS "start,stop,rms_channel_1,rms_channel_2\n"
#define OLMEGA_CSV_PSD_HEADINGS "start,stop,?\n"
#define OLMEGA_CSV_ZCR_HEADINGS "start,stop,?,?,?,?\n"

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
} olmega_feat_header;

typedef struct
{
    olmega_feat_header header;
    float *data;
} olmega_feat;

OLMEGADEF olmega_feat olmega_feat_create(uint8_t *data, size_t size);
OLMEGADEF void olmega_feat_destroy(olmega_feat feat);
OLMEGADEF void olmega_print_header_debug(olmega_feat_header header);
OLMEGADEF char* olmega_csv_string_create(olmega_feat feat, size_t *output_size);
OLMEGADEF void olmega_csv_string_destroy(char* csv_string);


#ifdef __cplusplus
}
#endif
    

//// IMPLEMENTATION ////
#if defined(OLMEGA_IMPLEMENTATION)

enum
{
    O32_LITTLE_ENDIAN = 0x03020100ul,
    O32_BIG_ENDIAN = 0x00010203ul,
    O32_PDP_ENDIAN = 0x01000302ul,      /* DEC PDP-11 (aka ENDIAN_LITTLE_WORD) */
    O32_HONEYWELL_ENDIAN = 0x02030001ul /* Honeywell 316 (aka ENDIAN_BIG_WORD) */
};

static const union { unsigned char bytes[4]; uint32_t value; } o32_host_order =
    { { 0, 1, 2, 3 } };

#define O32_HOST_ORDER (o32_host_order.value)


#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OLMEGA_IS_LITTLE_ENDIAN (O32_HOST_ORDER == O32_LITTLE_ENDIAN)

#ifndef OLMEGA_ASSERT
#include <assert.h>
#define OLMEGA_ASSERT(condition,message) assert(condition && message)
#endif

static inline void
swap_byte_order(uint8_t *value, size_t size)
{
    for(size_t low = 0, high = size - 1; low < high; low++, high--)
    {
        uint8_t tmp = value[high];
        value[high] = value[low];
        value[low]  = tmp;
    }
}

static inline int32_t
pop_int32(uint8_t *data, size_t *cursor)
{
    int32_t value = *(int32_t*)(data + *cursor);
    if(OLMEGA_IS_LITTLE_ENDIAN)
    {
        swap_byte_order((uint8_t*)&value,sizeof(value));
    }
    *cursor += sizeof(value);
    return(value);
}

static inline float
pop_float(uint8_t *data, size_t *cursor)
{
    float value = *(float*)(data + *cursor);
    if(OLMEGA_IS_LITTLE_ENDIAN)
    {
        swap_byte_order((uint8_t*)&value,sizeof(value));
    }
    *cursor += sizeof(value);
    return(value);
}

OLMEGADEF olmega_feat olmega_feat_create(uint8_t *data, size_t size)
{
    olmega_feat feat = {};
    size_t cursor = 0;

    feat.header.version = pop_int32(data,&cursor);
    feat.header.block_count = pop_int32(data,&cursor);
    feat.header.feature_dimensions = pop_int32(data,&cursor);
    feat.header.block_size = pop_int32(data,&cursor);
    feat.header.hop_size = pop_int32(data,&cursor);
    feat.header.sampling_rate = pop_int32(data,&cursor);

    memcpy(feat.header.sample_timestamp,data+cursor,sizeof(feat.header.sample_timestamp));
    cursor += sizeof(feat.header.sample_timestamp);

    memcpy(feat.header.system_timestamp,data+cursor,sizeof(feat.header.system_timestamp));
    cursor += sizeof(feat.header.system_timestamp);

    feat.header.calibration_channel_1 = pop_float(data,&cursor);
    feat.header.calibration_channel_2 = pop_float(data,&cursor);

    memcpy(feat.header.android_identifier,data+cursor,sizeof(feat.header.android_identifier));
    cursor += sizeof(feat.header.android_identifier);

    memcpy(feat.header.bluetooth_transmitter_mac,data+cursor,sizeof(feat.header.bluetooth_transmitter_mac));
    cursor += sizeof(feat.header.bluetooth_transmitter_mac);


    feat.data = (float*) malloc(sizeof(float) * feat.header.block_count * feat.header.feature_dimensions);
    OLMEGA_ASSERT(feat.data, "Allocation failure!");

    for(int32_t index = 0; index < feat.header.block_count * feat.header.feature_dimensions; index++)
    {
        feat.data[index] = pop_float(data,&cursor);
    }
    
    return(feat);
}

OLMEGADEF void olmega_feat_destroy(olmega_feat feat)
{
    if(feat.data)
    {
        free(feat.data);
    }
}

/*
OLMEGADEF char* olmega_csv_string_create(olmega_feat feat, size_t *output_size)
{
    const uint8_t precision = 4;
    const char* dummy_scientific_notation = "xE+000";
    size_t cursor = 0;
    
    uint32_t datum_string_length = strlen(dummy_scientific_notation) + (precision > 0 ? 1 : 0) + precision;
    uint32_t csv_string_length = ((datum_string_length+1) * feat.header.feature_dimensions) * feat.header.block_count;
    printf("CSV Length: %d\n",csv_string_length);
    char* output_csv = (char*) malloc(csv_string_length);
    OLMEGA_ASSERT(output_csv,"Allocation failure!");

    for(int32_t block = 0; block < feat.header.block_count; block++)
    {
        for(int32_t dimension = 0; dimension < feat.header.feature_dimensions; dimension++)
        {
            sprintf(output_csv + cursor,"%.*E,",precision,feat.data[block * feat.header.feature_dimensions + dimension]);
            cursor += datum_string_length+1;
        }
        cursor--;
        sprintf(output_csv + cursor,"\n");
        cursor++;
    }
    output_csv[cursor] = '\0';
    *output_size = cursor;

    return(output_csv);
}
*/

OLMEGADEF void olmega_csv_string_destroy(char* csv_string)
{
    if(csv_string)
    {
        free(csv_string);
    }
    csv_string = NULL;
}

/*
OLMEGADEF void olmega_print_header_debug(olmega_feat_header header)
{
    printf("Version: %d\nBlock Count: %d\nFeature Dimensions:%d\nBlock Size: %d\nHop Size: %d\nSampling Rate: %d\nSample Timestamp: %.16s\nSystem Timestamp: %.16s\nCalibration CH 1: %f\nCalibration CH 2: %f\nAndroid Identification: %.16s\nBluetooth Transmitter MAC: %.17s\n",
           header.version,
           header.block_count,
           header.feature_dimensions,
           header.block_size,
           header.hop_size,
           header.sampling_rate,
           header.sample_timestamp,
           header.system_timestamp,
           header.calibration_channel_1,
           header.calibration_channel_2,
           header.android_identifier,
           header.bluetooth_transmitter_mac
           );
}
*/
#ifdef __cplusplus
}
#endif

#endif //IMPLEMENTATION
#endif //OLMEGA_H
