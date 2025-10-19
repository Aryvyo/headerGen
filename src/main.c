#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define MAXLENGTH 512

//lazy header gen, i use PUBLIC as an attribute to determine what to emit
// #define PUBLIC __attribute__((annotate(\"header_export\")))
// not feature complete prob not safe, just made it to use rq, might revisit later

char *xstrdup(const char *str) {
    size_t n = strlen(str) + 1;
    char *ptr = (char *)malloc(n);
    if (ptr) memcpy(ptr, str, n);
    return ptr;
}

bool has_ext(const char *filename, const char *ext) {
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) return false;
    return strcmp(dot + 1, ext) == 0;
}

int main(int argc, char** argv){

	for (int fidx =1; fidx<argc; fidx++) {

		if (has_ext(argv[fidx], "c") || has_ext(argv[fidx], "C")) {
		

		char** lines = NULL;
		size_t lineCount = 0;
		size_t capacity = 0;

		FILE *fp;

		fp = fopen(argv[fidx],"r");

		if (fp==NULL) {
			return 1;
		}

		char buffer[MAXLENGTH];

		while (fgets(buffer,MAXLENGTH,fp)!=NULL) {
			int i = 0;

			while (buffer[i]==' ') i++; 

			if (memcmp(&buffer[i],"PUBLIC", 6)==0) {
				char* brace = strstr(buffer,"{");
				if (brace) *brace= '\0';

				char* comment = strstr(buffer,"//");
				if (comment) *comment = '\0';

				size_t len = strlen(buffer);
				while (len > 0 && buffer[len - 1] == ' ') {
				    buffer[--len] = '\0';
				}

				if (len>0 && buffer[len-1] != ';') {
					buffer[len++] = ';';
					buffer[len] = '\0';
				}

				if (lineCount >= capacity) {
				    capacity = capacity ? capacity * 2 : 16;
				    lines = realloc(lines, capacity * sizeof(char *));
				}

				lines[lineCount++] = xstrdup(buffer);
			}
		}

		fclose(fp);


		//first write 
		FILE* wf;

		char* filename = xstrdup(argv[fidx]);
		char *dot = strrchr(filename, '.');
		if (dot && strcmp(dot, ".c") == 0) strcpy(dot, ".h");

		wf = fopen(filename,"w");
		
		fprintf(wf, "#define PUBLIC __attribute__((annotate(\"header_export\")))\n");

		for (int i = 0; i<lineCount; i++) {
			printf("found Public function at %d\n%s",i,lines[i]);
			fprintf(wf,"%s\n",lines[i]);
		}

		}
		else {
			printf("fuck you\n");
		}


	}
	return 0;
}
