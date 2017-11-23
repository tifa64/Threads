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
    FILE * fp1, * fp2;
    char * line1 = NULL, * line2 = NULL;
    int mat1[1000][1000], mat2[1000][1000];
    int i = 0, j = 0;
    int rows1 = 0, *columns1 = 0, rows2 = 0, *columns2 = 0;
    size_t len = 0;
    ssize_t read1, read2;
    fp1 = fopen("Matrix1.txt", "r");
    if (fp1 == NULL)
        exit(EXIT_FAILURE);
    while ((read1 = getline(&line1, &len, fp1)) != -1) {
        rows1 ++;
        buildMat(mat1, i++, read1, line1, &columns1);
    }
    fclose(fp1);
    if (line1)
        free(line1);

    i = j = len = 0;
    fp2 = fopen("Matrix2.txt", "r");
    if (fp2 == NULL)
        exit(EXIT_FAILURE);
    while ((read2 = getline(&line2, &len, fp2)) != -1) {
        rows2 ++;
        buildMat(mat2, i++, read2, line2, &columns2);
    }
    fclose(fp2);
    if (line2)
        free(line2);

    intk;
    for (i = 0 ; i < rows1 ; i++){
        for(j = 0 ; j < columns2 ; j++){
            for()
        }
    }


    exit(EXIT_SUCCESS);
    return 0;
}
