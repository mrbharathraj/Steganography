/*
Name: Bharath Raj Buyyakar
Date: 7-02-2025
Description: Project on Steganography
*/
#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char **argv)
{
    if (check_operation_type(argc, argv) == e_encode)
    {
        printf("Selected operation is encoding\n");
        EncodeInfo encoding_structure;
        if (read_and_validate_encode_args(argv, &encoding_structure) == e_success)
        {
            printf("Read and validate encode function is success\n");
            printf("<-------Started to encode------->\n");
            if (do_encoding(&encoding_structure) == e_success)
            {
                printf("Encoding successfully completed\n");
            }
            else
            {
                printf("Failed to perform encoding\n");
                return e_failure;
            }
        }
        else
        {
            printf("Read and validate encode function failed\n");
            return e_failure;
        }
    }
    else if (check_operation_type(argc, argv) == e_decode)
    {
        printf("Selected operation is decoding\n");
        DecodeInfo decoding_structure;
        if (read_and_validate_decode_args(argv, &decoding_structure) == e_success)
        {
            printf("Read and validate decode function is success\n");
            printf("<-------Started to decode------->\n");
            if (do_decoding(&decoding_structure) == e_success)
            {
                printf("Decoding successfully completed\n");
            }
            else
            {
                printf("Failed to perform decoding\n");
                return e_failure;
            }
        }
        else
        {
            printf("Read and validate decode function failed\n");
            return e_failure;
        }
    }
    else
    {
        printf("Unsupported operation\n");
        return e_failure;
    }
    return e_success;
}
OperationType check_operation_type(int argc, char *argv[])
{
    if (argc < 2)
    {
        return e_unsupported;
    }
    if (strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}   else
    {
        return e_unsupported;
    }
}