#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * decoding secret file to decode.txt
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_DECODE_BUF_SIZE 1
#define MAX_DATA_BUF_SIZE (MAX_DECODE_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4
#define SIZE_OF_MAGIC_STRING 2
#define SIZE_OF_EXTN 4
#define SIZE_OF_SECRET_FILE 25

typedef struct _DecodeInfo
{
    
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
    char image_data[8];
    long file_extn_size;

    /* Decode File Info */
    char *decode_fname;
    FILE *fptr_decode;
    char decode_data[100];
    long size_decode_file;

    /* Magic Sting Info */
    char magic_string[SIZE_OF_MAGIC_STRING+1];

} DecodeInfo;


/* Decoding function prototype */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_files_for_decoding(DecodeInfo *decInfo);

/* Decode Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode lsb to integer */
Status decode_lsb_to_code_size(char *data, long *size);

/* Decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode LSB into byte of data */
Status decode_byte_from_lsb(char *data, char *data_buffer);

/* Decode the secret data and copy to decode.txt */
Status copy_decode_data(char *data, char *decoded_string);
#endif