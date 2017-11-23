#include <stdio.h>
#include <stdlib.h>
void buildMat(int mat[][1000], int i, int j, char *line, int *columns) {
    int iline = 0, jj = 0, k;
    char tempchar[j];
    int tempint = 0, col;
    for(col = 0; col < j; col++) {
		if(line[col] == ' ' || col == j-1) {
			if(tempchar != NULL) {
				tempint = 0;
	        	for(k = 0; k < iline; k++) {
		    		tempint = tempint * 10 + ( tempchar[k] - '0' );
    			}
				mat[i][jj++] = tempint;
			}
			iline = 0;
		}
		else
    		tempchar[iline++] = line[col];
    	}
    *columns = jj;
}
int main()
{
    FILE * fp;
    char * line = NULL;
    int mat[1000][1000];
    int i = 0, j = 0;
    int rows = 0, *columns = 0;
    size_t len = 0;
    ssize_t read;
    fp = fopen("Matrix.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    while ((read = getline(&line, &len, fp)) != -1) {
        rows ++;
        buildMat(mat, i++, read, line, &columns);
    }
    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
    return 0;
}
