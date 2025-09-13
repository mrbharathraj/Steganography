#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>
#include "common.h"

//Validate whether proper .bmp and .txt files are passed as CLA
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    //Check whether the 3rd argument passed as CLA is .bmp file
    if(strstr(argv[2],".bmp")!=NULL)
    {
        //Store stego.bmp file name into stego_image_fname
        decInfo->stego_image_fname=argv[2];
    }
    else
        return e_failure;

    //Check if .txt file is passed as an argument or not
    if(argv[3]!=NULL)
        decInfo->decode_fname=argv[3]; //Store that file name into decode_fname
    else
        decInfo->decode_fname="decode.txt";  //Create decode.txt file
    
    // No failure return e_success
    return e_success;
}

// Perform all the  operations of decoding here
Status do_decoding(DecodeInfo *decInfo)
{
    // Open files for performing decoding
    if(open_files_for_decoding(decInfo)==e_success)
    {
        printf("Open files is a success\n");
    }
    else
    {
        printf("Open files is a failure\n");
        return e_failure;
    }
    // Decoding magic string from stego.bmp
    if(decode_magic_string(decInfo)==e_success)
    {
        printf("Decoding magic string is a success\n");
    }
    else
    {
        printf("Decoding magic string is a failure\n");
        return e_failure;
    }
    // Decode the secret file extension size
    if(decode_secret_file_extn_size(decInfo)==0)
    {
        printf("Decoding secret file extension size is a success\n");
    }
    else
    {
        printf("Decoding secret file extension size is a failure\n");
        return e_failure;
    }
    // Decode the secret file extension
    if(decode_secret_file_extn(decInfo)==0)
    {
        printf("Decoding secret file extension is a success\n");
    }
    else
    {
        printf("Decoding secret file extension is a failure\n");
        return e_failure;
    } 
    // Decoding secret file size
    if(decode_secret_file_size(decInfo)==e_success)
    {
        printf("Decoding secret file size is a success\n");
    }
    else
    {
        printf("Decoding secret file size is a failure\n");
        return e_failure;
    }
    // Decoding secret file data
    if(decode_secret_file_data(decInfo)==e_success)
    {
        printf("Decoding secret file data is a success\n");
    }
    else
    {
        printf("Decoding secret file data is a failure\n");
        return e_failure;
    }

    // No failure return e_success
    return e_success;
}

/* To open the necessary files for decoding */ 
Status open_files_for_decoding(DecodeInfo *decInfo)
{

    /* Stego Image file */
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
        // If failed to open the file, return e_failure
    	return e_failure;
    }

    /* Decode file */
    decInfo->fptr_decode = fopen(decInfo->decode_fname, "w");
    // Do Error handling
    if (decInfo->fptr_decode == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->decode_fname);
        // If failed to open the file, return e_failure
    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

/* To decode the magic string */
Status decode_magic_string(DecodeInfo *decInfo)
{
    //To extract the magic string from the stego.bmp image
    //Seek to 54th byte
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    //Read the size of the magic string
    int size=strlen(MAGIC_STRING);
    //printf("%d\n",size); //2
    // Run the loop for size no. of times of the magic string
    for(int i=0; i<size; i++)
    {
        //Read 8 bytes of stego.bmp to data_image
        fread(decInfo->image_data, 8, 1, decInfo->fptr_stego_image);
        // Decode each byte from LSB
        if (decode_byte_from_lsb(decInfo->image_data, &decInfo->magic_string[i]) != e_success)
        {  
            return e_failure;
        }
    }
    // Add '\0' to make as a string
    decInfo->magic_string[2]='\0';
    //Compare the decoded magic string data with the magic string
    if(strcmp(decInfo->magic_string, MAGIC_STRING)==0) //#*
        return e_success;
    else
        return e_failure;    
}

/* Decoding of byte from the lsb */       
Status decode_byte_from_lsb(char *data, char *decoded_string)
{
     char res=0;
     for(int i=0; i<8; i++)
     {
        res= res | ((data[i] & 0x01)<<(7-i));
     }  
     *decoded_string=res;
     //printf("%c",decoded_string); //#
     return e_success;
   
}

/* Decoding of secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
	char extn_size[32];
    // Read 32 bytes of stego.bmp
	fread(extn_size, 32, 1, decInfo->fptr_stego_image);
    // Decode lsb to code size
	decode_lsb_to_code_size(extn_size, &decInfo->file_extn_size);
    if((decInfo->file_extn_size)!=SIZE_OF_EXTN) //4
        return e_failure;
    else
        return e_success;
}    

/* Decoding of secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char file_extn[32];
    //Extension (.txt) is of 4 chars stored in 32 bytes of stego.bmp
	for (int i=0; i<decInfo->file_extn_size; i++)
	{
        //Read 32 bytes of stego.bmp
		fread(file_extn, 8, 1, decInfo->fptr_stego_image);
        // Decode byte from lsb 
	    decode_byte_from_lsb(decInfo->stego_image_fname, file_extn);    
	}
    if(strstr(decInfo->decode_fname, file_extn)!=NULL) //.txt
    {
        return e_success;
    }   
}

/* Decoding of secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char file_size[32];
    fread(file_size, 32, 1, decInfo->fptr_stego_image);
    
    decode_lsb_to_code_size(file_size, &decInfo->size_decode_file);
    if(decInfo->size_decode_file!=SIZE_OF_SECRET_FILE) //25
        return e_failure;
    else
        return e_success;
}
/* Decode lsb to code size */
Status decode_lsb_to_code_size(char *data, long *size)
{
    int res=0;
    for(int i=0; i<32; i++)
    {
        res= res | ((data[i] & 0x01)<<(31-i));
    }
    *size=res;
    //printf("%ld\n",*size);
}

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char decoded_secret_data[32];
    // Run the loop for 25 times to decode the secret file data
    for(int i=0; i<decInfo->size_decode_file; i++)
    {
        // Read 8 bytes from stego.bmp 
        fread(decInfo->decode_data, 8, 1, decInfo->fptr_stego_image);
        // Decode the secret data
        copy_decode_data(decInfo->decode_data, decoded_secret_data);
        // Write the decoded data to decode.txt file
        fwrite(decoded_secret_data, 1, 1, decInfo->fptr_decode);
    }
    return e_success;
}
/* Decode secret data and copy to decode.txt file */
Status copy_decode_data(char *data, char *decoded_string)
{
    char res=0;
    for(int i=0; i<8; i++)
    {
       res= res | ((data[i] & 0x01)<<(7-i));
    }  
     *decoded_string=res;
    //printf("%c",*decoded_string);
}