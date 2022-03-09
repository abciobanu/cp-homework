/* Ciobanu Andrei-Bogdan 312CB */

#include "solver.h"

#include <stdlib.h>
#include <string.h>

#define ARGS_NUM 3

int main(int argc, char *argv[])
{
    if (argc == ARGS_NUM) {
        /* extrage calea catre input */
        char *input_path = argv[1];

        /* extrage task-ul */
        char *task = argv[2];

        /* rezolvare task */
        if (strcmp(task, "123") == 0) {
            int result = solve_task123(input_path);

            return result;
        }
    }

    return 0;
}
