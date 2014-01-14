// Copyright (c) Sergey Lyubka, 2013.
// All rights reserved.

// This program is used to embed arbitrary data into a C binary. It takes
// a list of files as an input, and produces a .c data file that contains
// contents of all these files as collection of char arrays.
// Usage:
//   1. Compile this file:
//      cc -o embed embed.c
//
//   2. Convert list of files into single .c:
//      ./embed file1.data file2.data > embedded_data.c
//
//   3. In your application code, you can access files using this function:
//
//      const char *find_embedded_file(const char *file_name, size_t *size);
//      size_t size;
//      const char *data = find_embedded_file("file1.data", &size);
//
//   4. Build your app with embedded_data.c:
//      cc -o my_app my_app.c embedded_data.c

#include <stdio.h>
#include <stdlib.h>

static const char *code =
	"const char *find_embedded_file(const char *name, size_t *size) {\n"
	"\tconst struct embedded_file *p;\n"
	"\tfor (p = embedded_files; p->name != NULL; p++) {\n"
	"\t\tif (!strcmp(p->name, name)) {\n"
	"\t\t\tif (size != NULL) { *size = p->size; }\n"
	"\t\t\treturn (const char *) p->data;\n"
	"\t\t}\n"
	"\t}\n"
	"\treturn NULL;\n"
	"}\n";

int main(int argc, char *argv[]) {
	FILE *fp;
	int i, j, ch;

	for (i = 1; i < argc; i++) {
		if ((fp = fopen(argv[i], "rb")) == NULL) {
			exit(EXIT_FAILURE);
		} else {
			printf("#include <stddef.h>\n\n"
				"static const unsigned char v%d[] = {", i);
			for (j = 0; (ch = fgetc(fp)) != EOF; j++) {
				if ((j % 10) == 0) {
					printf("%s", "\n\t");
				}
				printf("%#04x, ", ch);
			}
			// Append zero byte at the end, to make text files appear in memory
			// as nul-terminated strings.
			printf("%s", " 0x00\n};\n");
			fclose(fp);
		}
	}

	printf("%s", "\nconst struct embedded_file {\n");
	printf("%s", "\tconst char *name;\n");
	printf("%s", "\tconst unsigned char *data;\n");
	printf("%s", "\tsize_t size;\n");
	printf("%s", "} embedded_files[] = {\n");

	for (i = 1; i < argc; i++) {
		printf("  {\"%s\", v%d, sizeof(v%d) - 1},\n", argv[i], i, i);
	}
	printf("%s", "\t{NULL, NULL, 0}\n");
	printf("%s", "};\n\n");
	printf("%s", code);

	return EXIT_SUCCESS;
}
