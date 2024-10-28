#ifndef __BUILD_JUST_H__
#define __BUILD_JUST_H__

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <sched.h>
#include <sys/types.h>
#include <stdint.h>

#define NOT_IMPLEMENTED(msg) do { fprintf(stderr, "%s:%d: To implement: %s\n", __FILE__, __LINE__, msg); abort(); } while(0)
#define ERROR(msg) do { fprintf(stderr, "%s:%d: ERROR: %s\n", __FILE__, __LINE__, msg); abort(); } while(0)

#define ARRAY_LEN(array) (sizeof(array)/sizeof(array[0]))
#define SAFE_ARRAY_GET(array, index) (assert((size_t)index < ARRAY_LEN(array)), array[(size_t)index])

typedef char* String;
typedef const char* CString;

int RUN_COMMAND(CString* command, String* output, size_t output_size);
int run_command();

void REBUILD_YOURSELF();

#endif // __BUILD_JUST_H__

#ifndef __CRC32_H__
#define __CRC32_H__

#define SOURCE_FILE "example.c"
#define CHECKSUM_FILE "./buildjust/example_checksum.txt"
#define CRC32_TABLE_SIZE 256

uint32_t crc32_table[CRC32_TABLE_SIZE];

void init_crc32_table() {
    uint32_t crc;
    for (uint32_t i = 0; i < CRC32_TABLE_SIZE; i++) {
        crc = i;
        for (size_t j = 8; j > 0; j--) {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;
            else
                crc >>= 1;
        }
        crc32_table[i] = crc;
    }
}

uint32_t crc32(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) ERROR("File opening failed");

    uint32_t crc = 0xFFFFFFFF;
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        crc = (crc >> 8) ^ crc32_table[(crc ^ ch) & 0xFF];
    }
    fclose(file);
    return crc ^ 0xFFFFFFFF;
}

void save_checksum(const char* checksum_filename, uint32_t checksum) {
    FILE* file = fopen(checksum_filename, "w");
    if (!file) ERROR("Failed to save checksum file");
    fprintf(file, "%u", checksum);
    fclose(file);
}

void read_checksum(const char* checksum_filename, uint32_t* checksum) {
    FILE* file = fopen(checksum_filename, "r");
    if (!file) return;//ERROR("Failed to read checksum file");
    fscanf(file, "%u", checksum);
    fclose(file);
}

int check_checksum(const char* source_file, const char* checksum_file) {
    init_crc32_table();
    uint32_t new_checksum = crc32(source_file);
    printf("New checksum: %u\n", new_checksum);

    uint32_t old_checksum;
    read_checksum(checksum_file, &old_checksum);
    printf("Old checksum: %u\n", old_checksum);
    if (new_checksum == old_checksum) {
        printf("File has not changed.\nContinuie execution.\n");
        return 0;
    } else {
        printf("The file has been modified.\nRebuilding Itself.\n");
        //save_checksum(checksum_file, new_checksum);
        return 1;
    }
}

#endif // __CRC32_H__

int RUN_COMMAND(CString* command, String* output, size_t output_size) {}

int run_command() {}

void REBUILD_YOURSELF() {
    if (check_checksum(SOURCE_FILE, CHECKSUM_FILE)) {
	printf("File has changed\n");
	printf("Rebuilding...\n");
	
	system("gcc -o ./bin/example ");
    } else {
	printf("File hasn't changed\n");
    }
    // if different create new
    //   compile
    //     get compile settings
    //     compiler, flags, argumetns, output, source...
    //   run new
    // if not
    //   run old
}
