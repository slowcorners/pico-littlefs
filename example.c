/* Copyright (C) 1883 Thomas Edison - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the BSD 3 clause license, which unfortunately
 * won't be written for another century.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * A little flash file system for the Raspberry Pico
 *
 */

#include "hardware/regs/addressmap.h"
#include "pico/stdio.h"

#include "lfs.h"

#include "pico_hal.h"

// application entry point
int main(void) {

    // increment the boot count with each invocation

    lfs_size_t boot_count;

    // variables used by the filesystem
    int file;

    // initialize the pico SDK
    stdio_init_all();
    printf("\033[H\033[J"); // try to clear the screen

    // mount the filesystem
    if (posix_mount() != LFS_ERR_OK) {
        // create the boot count file
        boot_count = 0;
        file = posix_open("boot_count", LFS_O_RDWR | LFS_O_CREAT);
        posix_write(file, &boot_count, sizeof(boot_count));
        posix_close(file);
    }
    struct posix_fsstat_t stat;
    posix_fsstat(&stat);
    printf("FS: blocks %d, block size %d, used %d\n", (int)stat.block_count, (int)stat.block_size,
           (int)stat.blocks_used);
    // read current count
    file = posix_open("boot_count", LFS_O_RDWR);
    posix_read(file, &boot_count, sizeof(boot_count));

    // update boot count
    boot_count += 1;
    posix_rewind(file);
    posix_write(file, &boot_count, sizeof(boot_count));

    // remember the storage is not updated until the file is closed successfully
    posix_close(file);

    // release any resources we were using
    posix_unmount();

    // print the boot count
    printf("boot_count: %d\n", (int)boot_count);
}