/*
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 *   Project Name:  Stegnography
 *
 *    Description:  Encryption and decription of data in .wav music file.
 *
 *        Created:  22 Feb 2016  18:41:27
 *       Compiler:  gcc
 *
 *         Author:  Praveen Kumar K
 *
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 */

#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include "Utils/functions.h"

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  Recieves the commandline arguments and checks the errors in input.
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
    int i, k;
    char *inputs[SIZE] = {0};      /* [0] = options, [1] = data file, [2] = wav file, [3] = output file */

    /*Save the command line arguments*/
    for (i = 1, k = 1; i < argc; i++)
    {
        if (*argv[i] == '-')
        {
            if (inputs[0] != 0)
            {
                PRINT_ERR_USAGE;
            }

            inputs[0] = argv[i];
        }
        else
        {
            inputs[k] = argv[i];
            k++;
        }
    }

    /*Validate the option choosed by user*/
    if (inputs[0] != 0 && argc == 5 && !strcmp(inputs[0], "-e"))
    {
        encryption(inputs[1], inputs[2], inputs[3]);
    }
    else if (inputs[0] != 0 && argc == 4 && !strcmp(inputs[0], "-d"))
    {
        decryption(inputs[1], inputs[2]);
    }
    else if (inputs[0] != 0 && argc == 2 && !strcmp(inputs[0], "--help"))
    {
        print_help(argv[0]);
    }
    else
    {
        PRINT_ERR_USAGE;
    }


    return 0;
}
