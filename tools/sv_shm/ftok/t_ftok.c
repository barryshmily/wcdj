/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2010.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU Affero General Public License as published   *
* by the Free Software Foundation, either version 3 or (at your option)   *
* any later version. This program is distributed without any warranty.    *
* See the file COPYING.agpl-v3 for details.                               *
\*************************************************************************/

/* t_ftok.c

   Test the key values returned by ftok(3).

   Usage: t_ftok key-file key-char

   Simply calls ftok(), using the values supplied in the command-line arguments,
   and displays the resulting key.
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/stat.h>

int
main(int argc, char *argv[])
{
    key_t key;
    struct stat sb;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
	{
        printf("%s file-name keychar\n", argv[0]);
		return 0;
	}

    printf("Size of key_t = %ld bytes\n", (long) sizeof(key_t));

    if (stat(argv[1], &sb) == -1)
	{
        printf("stat error[%d:%s]\n", errno, strerror(errno));
		return -1;
	}

	int iproj_id = atoi((char *)argv[2]);
    key = ftok(argv[1], iproj_id);
    if (key == -1)
	{
        printf("ftok error[%d:%s]\n", errno, strerror(errno));
		return -1;
	}

    printf("Key = %lx i-node = %lx st_dev = %lx proj = %x\n",
          (unsigned long) key, (unsigned long) sb.st_ino,
          (unsigned long) sb.st_dev, iproj_id);
    if (key == -1)
        printf("File does not exist\n");

    exit(EXIT_SUCCESS);
}

// g++ -o t_ftok t_ftok.c
