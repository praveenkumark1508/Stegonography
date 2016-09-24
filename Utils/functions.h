#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//Colours
#define RED     "[1;31m"
#define GREEN   "[1;32m"
#define YELLOW  "[1;33m"
#define BLUE    "[1;34m"
#define MAGENTA "[1;35m"
#define CYAN    "[1;36m"
#define RESET   "[0m"

#define SIZE    4
#define BYTE    7
#define PRINT_ERR_USAGE printf(RED"Error in usage!!\nTry '%s --help' for more information\n"RESET, argv[0]); exit(1)

/*Structures*/
typedef union
{
    unsigned char byte;
    unsigned char bit:1;
} Data_t;

typedef union
{
    unsigned int word;
    unsigned int bit:1;
} Data2_t;

/*Function prototypes*/
void print_help(char *app);
void encryption(char *data_file, char *wav_file, char *out_file);
void decryption(char *wav_file, char *out_file);
void open_in_files(FILE **fptr, char *file);
void open_out_files(FILE **fptr, char *file);
long get_data_file_size(FILE *fptr_data);
long get_embedded_size(FILE **fptr, int location);
int get_audio_size(FILE *fptr);
void copy_wav(FILE **fptr_wav, FILE **fptr_out, int limit);
void insert_size_of_data(int size, FILE **fptr_wav, FILE **fptr_out);
void encrypt_data(FILE **fptr_data, FILE **fptr_wav, FILE **fptr_out);
void decrypt_data(FILE **fptr_wav, FILE **fptr_out, int size);
int file_exist_check(char *file);

#endif
