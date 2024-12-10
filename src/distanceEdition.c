#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "algorithms.h"
#include "characters_to_base.h"

static char* map_file(const char* filename, size_t* file_size) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(1);
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("Error getting file size");
        close(fd);
        exit(1);
    }

    *file_size = sb.st_size;
    char* addr = mmap(NULL, *file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    if (addr == MAP_FAILED) {
        perror("Error mapping file");
        exit(1);
    }

    return addr;
}

int main(int argc, char* argv[]) {
    if (argc != 7) {
        fprintf(stderr, "Usage: %s file1 b1 L1 file2 b2 L2\n", argv[0]);
        return 1;
    }

    // We parse the arguments
    const char* file1 = argv[1];
    size_t b1 = atol(argv[2]);
    size_t L1 = atol(argv[3]);
    const char* file2 = argv[4];
    size_t b2 = atol(argv[5]);
    size_t L2 = atol(argv[6]);

    // We map the files
    size_t file1_size, file2_size;
    char* content1 = map_file(file1, &file1_size);
    char* content2 = map_file(file2, &file2_size);

    // We check that the sequences are within the file bounds
    if (b1 + L1 > file1_size || b2 + L2 > file2_size) {
        fprintf(stderr, "Error: sequence lengths exceed file sizes\n");
        munmap(content1, file1_size);
        munmap(content2, file2_size);
        return 1;
    }

    // We run the algorithms
    AlignmentResult res1 = iterative_alignment(content1 + b1, L1, content2 + b2, L2);
    printf("Iterativo: distancia=%d, operaciones=%zu, accesos=%zu\n", 
           res1.distance, res1.operations, res1.memory_accesses);

    AlignmentResult res2 = cache_aware_alignment(content1 + b1, L1, content2 + b2, L2, 4096, 64);
    printf("Cache-aware: distancia=%d, operaciones=%zu, accesos=%zu\n", 
           res2.distance, res2.operations, res2.memory_accesses);

    AlignmentResult res3 = cache_oblivious_alignment(content1 + b1, L1, content2 + b2, L2);
    printf("Cache-oblivious: distancia=%d, operaciones=%zu, accesos=%zu\n", 
           res3.distance, res3.operations, res3.memory_accesses);

    // We check that the results are consistent
    if (res1.distance != res2.distance || res1.distance != res3.distance) {
        fprintf(stderr, "Warning: Resultados inconsistentes entre implementaciones\n");
    }

    // We unmap the files
    munmap(content1, file1_size);
    munmap(content2, file2_size);

    return 0;
}