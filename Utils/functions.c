#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "functions.h"


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  print_help
 *  Description:  Prints the instructions about how to use the program
 * =====================================================================================
 */
void print_help(char *app)
{
    printf("\e[1;1H \e[J");
    printf(CYAN"---STEGANOGRAPHY HELP---\n\n\n"RESET
            GREEN"syntax for encryption"RESET": %1$s -e <data_file> <audio_file> <output_file>\n\n"
            GREEN"syntax for decryption"RESET": %1$s -d <encrypted_audio_file> <output_file>\n\n", app);
}


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  encryption
 *  Description:  Guides the encryption process.
 * =====================================================================================
 */
void encryption(char *data_file, char *wav_file, char *out_file)
{
    long wav_size, data_size;
    FILE *fptr_data, *fptr_wav, *fptr_out;

    /*Open files*/
    open_in_files(&fptr_data, data_file);
    open_in_files(&fptr_wav, wav_file);
    open_out_files(&fptr_out, out_file);

    /*Find the size of the audio file*/
    wav_size = get_audio_size(fptr_wav);

    /*Find the size of the data file*/
    data_size = get_data_file_size(fptr_data);

    /*If the size of the data in audio file is 8 times greater then data file, then start encoding*/
    rewind(fptr_data);
    rewind(fptr_wav);
    rewind(fptr_out);

    if (8 * data_size <= wav_size)
    {
        copy_wav(&fptr_wav, &fptr_out, 45);
        insert_size_of_data(data_size, &fptr_wav, &fptr_out);
        encrypt_data(&fptr_data, &fptr_wav, &fptr_out);
        copy_wav(&fptr_wav, &fptr_out, 0x0FFFFFFF);
    }
    else
    {
        printf(RED"Size of the audio file is too small to encrypt the data\n"RESET);
    }

    fclose(fptr_data);
    fclose(fptr_wav);
    fclose(fptr_out);
}


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  decryption
 *  Description:  Guides the decryption process.
 * =====================================================================================
 */
void decryption(char *wav_file, char *out_file)
{
    long data_size;
    FILE *fptr_wav, *fptr_out;

    /*Open files*/
    open_in_files(&fptr_wav, wav_file);
    open_out_files(&fptr_out, out_file);

    /*Find the size of the data file*/
    data_size = get_embedded_size(&fptr_wav, 45);

    decrypt_data(&fptr_wav, &fptr_out, data_size);

    fclose(fptr_wav);
    fclose(fptr_out);
}


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  open_in_files
 *  Description:  Open input files and does proper error checking.
 * =====================================================================================
 */
void open_in_files(FILE **fptr, char *file)
{
    /*Validate the files*/
    if (!(*fptr = fopen(file, "r")))
    {
        char msg[100];
        sprintf(msg, RED"ERROR : %s"RESET, file);

        perror(msg);
        exit(1);
    }
}


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  open_out_files
 *  Description:  Open output files and does proper error checking.
 * =====================================================================================
 */
void open_out_files(FILE **fptr, char *file)
{
    if ((file_exist_check(file) == 1) && !(*fptr = fopen(file, "w+")))
    {
        printf(RED"ERROR: Cannot create the file %s\n"RESET, file);
        exit(1);
    }
}


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_data_file_size
 *  Description:  Finds the size of the data to be encrypted
 * =====================================================================================
 */
long get_data_file_size(FILE *fptr_data)
{
    long data_size;

    /*Check the size of the wav file is sufficient to store the data*/
    fseek (fptr_data, 0, SEEK_END);
    data_size = ftell(fptr_data);

    return data_size;
}


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_wave_file_size
 *  Description:  Finds the size of the data in audio file.
 * =====================================================================================
 */
long get_embedded_size(FILE **fptr, int location)
{
    int i, temp, size = 0;

    fseek (*fptr, location, SEEK_SET);

    for (i = 0; i < 32; i++)
    {
        if (fread(&temp, 1, 1, *fptr) > 0)
        {
            size = size | ((temp & 1) << i);
        }
        else
            return -1;
    }

    return size;
}


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_wave_file_size
 *  Description:  Finds the size of the data in audio file.
 * =====================================================================================
 */
int get_audio_size(FILE *fptr)
{
    long size;

    fseek (fptr, 0, SEEK_END);
    size = ftell(fptr) - 44;

    return size;
}
/*
 * ===  FUNCTION  ======================================================================
 *         Name:  encrypt_data
 *  Description:  Does the encryption of the data
 * =====================================================================================
 */
void encrypt_data(FILE **fptr_data, FILE **fptr_wav, FILE **fptr_out)
{
    Data_t data_byte, write_byte;
    int i;


    while (fread(&(data_byte.byte), 1, 1, *fptr_data) > 0)
    {
        for (i = 0; i < 8; i++)      //take one bit from data and encrypt it
        {
            write_byte.byte = fgetc(*fptr_wav);
            write_byte.bit = data_byte.bit;
            data_byte.byte >>= 1;

            fwrite(&(write_byte.byte), 1, 1, *fptr_out);
        }
    }
}


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  decrypt_data
 *  Description:  Does the decryption of the data
 * =====================================================================================
 */
void decrypt_data(FILE **fptr_wav, FILE **fptr_out, int size)
{
    int i, k, temp, write_byte;

    for (k = 0; k < size; k++)
    {
        write_byte = 0;

        for (i = 0; i < 8; i++)
        {
            if (fread(&temp, 1, 1, *fptr_wav) > 0)
                write_byte = write_byte | ((temp & 1) << i);
        }

        fwrite(&write_byte, 1, 1, *fptr_out);
    }
}
/*
 * ===  FUNCTION  ======================================================================
 *         Name:  insert_size_of_data
 *  Description:  inserts the size of the data to be encrypted to the output file at the
 *                start of the encryption.
 * =====================================================================================
 */
void insert_size_of_data(int size, FILE **fptr_wav, FILE **fptr_out)
{
    Data2_t data;
    Data_t write_byte;
    int i;

    data.word = size;

    for (i = 0; i < 32; i++)      //take one bit from data and encrypt it
    {
        write_byte.byte = fgetc(*fptr_wav);
        write_byte.bit = data.bit;
        data.word >>= 1;

        fwrite(&(write_byte.byte), 1, 1, *fptr_out);
    }
}


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  copy_wav
 *  Description:  Copies the given number of bytes from audio file as it is into output
 *                file
 * =====================================================================================
 */
void copy_wav(FILE **fptr_wav, FILE **fptr_out, int limit)
{
    int i;
    char write_byte;

    for (i = 0; i < limit; i++)
    {
        if (fread(&write_byte, sizeof(char), 1, *fptr_wav) > 0)
            fwrite(&write_byte, sizeof(char), 1, *fptr_out);
        else
            break;
    }
}


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  file_exist_check
 *  Description:  Checks if the file is present in current directory or not
 * =====================================================================================
 */
int file_exist_check(char *file)
{
    if (access(file, F_OK) != -1)
    {
        printf(RED"%s: file already exists. Do you want to overwrite it? (y/N) : "RESET, file);

        if (getchar() != 'y')
        {
            printf(RED"Exiting the program\n"RESET);
            exit(1);
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }
}
