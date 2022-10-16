#include "../include/utils.h"

#include <stdio.h>
#include <stdlib.h>

void ExitError(char* message)
{
    printf("[Error] %s", message);
    system("PAUSE");
    exit(-1);
}