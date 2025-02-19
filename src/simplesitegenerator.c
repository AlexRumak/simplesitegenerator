#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "run_tests.h"

#define NUMBER_OF_FLAGS 2

enum Flag
{
    RUN_TESTS,
    RUN_MAIN
};

static int contains_flag(enum Flag flag, int flagc, enum Flag *flags)
{
    for (int i = 0; i < flagc; i++)
    {
        if (flags[i] == flag) 
        {
            return 1;
        }
    }
    return 0;
}

static enum Flag *parseFlags(int argc, char **argv, int *numFlags)
{
    int arrSize = NUMBER_OF_FLAGS;
    *numFlags = 0;
    enum Flag *flags = malloc(arrSize * sizeof(enum Flag));
    
    for(int i = 1; i < argc; i++)
    {
        char* argument = argv[i];
        if (argument[0] == '-')
        {
            argument++;
            if (argument[0] == 'M') {
                argument++;
                if (!strcmp(argument, "run_test_suite"))
                {
                    flags[(*numFlags)++] = RUN_TESTS;
                }
            }
        }
    }

    return flags;
}

int main(int argc, char **argv) {
    int numFlags;
    enum Flag *flags = parseFlags(argc, argv, &numFlags);

    // Run Unit Test Suite
    if (contains_flag(RUN_TESTS, numFlags, flags))
    {
        run_tests();
        exit(0);
    }

    // MAIN
    FILE *fp;
    char s[1024];
    int linecount = 0;

    fp = fopen("settings.yml", "r");

    while (fgets(s, sizeof s, fp) != NULL) {
        printf("%d: %s", ++linecount, s); 
    }

    fclose(fp);
}