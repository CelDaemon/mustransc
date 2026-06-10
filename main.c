#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 4096

char const *path_to_filename(char const * const path) {
    auto const match = strrchr(path, '/');
    if (!match)
        return path;
    return match + 1;
}

int str_hash(char const * const str) {
    auto const len = strlen(str);
    auto acc = 0;
    for (int i = 0; i < len; i++)
        acc = str[i] + 31 * acc;
    return acc;
}

void decode_buffer(char * restrict const buf, int const len, int * restrict const key) {
    auto cur = *key;
    for(int i = 0; i < len; i++) {
        char const dec = buf[i] = (char)(buf[i] ^ (cur >> 8));
        cur = cur * 498729871 + 85731 * dec;
    }
    *key = cur;
}

int decode(FILE * restrict const input, FILE * restrict const output, int const key) {
    auto cur = key;
    char buffer[BUFFER_SIZE] = {};
    auto read = 0;
    do {
        read = fread(buffer, 1, BUFFER_SIZE, input);
        decode_buffer(buffer, read, &cur);
        if(fwrite(buffer, 1, read, output) != read)
            return 1;
    } while (read == BUFFER_SIZE);
    return ferror(input);
}

int encode_decode(char const * const arg) {
    if(!strcmp(arg, "decode"))
        return 0;
    if(!strcmp(arg, "encode"))
        return 1;
    return -1;
}

int calculate_key(char const * restrict const path, char const * restrict const key) {
    auto const str = key ? key : path_to_filename(path);
    return str_hash(str);
}


int main(int argc, char *argv[]) {
    if (argc < 4) {
        fputs("Usage: musdec <decode|encode> [INPUT] [OUTPUT] (KEY)\n", stderr);
        return 1;
    }

    auto const encode = encode_decode(argv[1]);
    if(encode == -1) {
        fprintf(stderr, "Invalid action '%s', must be decode or encode", argv[1]);
        return 1;
    }

    auto const input_path = argv[2];
    auto const input_file = fopen(input_path, "rb");
    if (!input_file) {
        perror("Failed to input file");
        return 1;
    }

    auto const output_path = argv[3];
    auto const output_file = fopen(output_path, "wb");
    if (!output_file) {
        perror("Failed to open output file");
        fclose(input_file);
        return 1;
    }

    auto const key_arg = argc > 4 ? argv[4] : NULL;
    auto const mus_path = !encode ? input_path : output_path;
    auto const key = calculate_key(mus_path, key_arg);

    printf("%d\n", key);

    if (decode(input_file, output_file, key)) {
        perror("Failed to decode file");
        fclose(input_file);
        fclose(output_file);
        return 1;
    }
    fclose(input_file);
    fclose(output_file);
    puts("Decoding succesful");

    return 0;
}

