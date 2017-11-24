#include <stdio.h>
#include <stdlib.h>
/**
Implementation of a multi-threaded matrix multiplication program.
**/
//Passing the array, row# and length of the line read from the matrix
//The columns are calculated inside the function, they are passed as pointer since the funcation is void
void buildMat(float mat[][10], int i, int j, char *line, int *columns) {
    //A temprorary string stores the characters from the line till it hit a space
    int iline = 0, jj = 0, col;
    char tempchar[j];
    float tempint = 0;
    //Then it is converted to a float value and the matrix is built
    for(col = 0; col < j; col++) {
        //We know the end of the number
		if(line[col] == ' ' || col == j-1) {
            //To prevent exceptions
			if(tempchar != NULL) {
                //Convert it from string to float
				tempint = atof(tempchar);
				//Adding it to the matrix
				mat[i][jj++] = tempint;
			}
			iline = 0;
		}
		//This allows us to read matrices without knowing rows# or cols# beforehand
		else
    		tempchar[iline++] = line[col];
    	}
    //We we set the value of the columns
    *columns = jj;
}
int main()
{
    //Three pointer files fot the 3 matrices
    FILE *fp1, *fp2, *fp3;
    //To read from Matrices one and two
    char *line1 = NULL, *line2 = NULL;
    //The matrices
    float mat1[10][10], mat2[10][10], mat3[10][10];
    //Used for later iterations
    int i = 0, j = 0, k = 0;
    //Rows and columns for the 2 input matrices
    int rows1 = 0, columns1 = 0, rows2 = 0, columns2 = 0;
    //Length of the line which helps us calculating the columns#
    size_t len = 0;
    //We use a technique to read the file line by line
    ssize_t read1, read2;
    fp1 = fopen("Matrix1.txt", "r");
    //To prevent exceptions
    if (fp1 == NULL)
        exit(EXIT_FAILURE);
    //Where we actually read the file
    while ((read1 = getline(&line1, &len, fp1)) != -1) {
        //This is how we calculate the rows
        rows1 ++;
        //Calling the function which convert that line into a 2D float array
        buildMat(mat1, i++, read1, line1, &columns1);
    }
    //Closing the file and get rid of the string
    fclose(fp1);
    if (line1)
        free(line1);
    //Resetting everything for the next file
    i = j = len = 0;
    fp2 = fopen("Matrix2.txt", "r");
    //Same precautions
    if (fp2 == NULL)
        exit(EXIT_FAILURE);
    //Same drill
    while ((read2 = getline(&line2, &len, fp2)) != -1) {
        rows2 ++;
        buildMat(mat2, i++, read2, line2, &columns2);
    }
    fclose(fp2);
    if (line2)
        free(line2);
    //Resetting the resulted matrix
    for(i = 0; i < rows1; i++){
        for(j = 0; j < columns1; j++)
            mat3[i][j] = 0;
    }
    //Calculate multiplication of the 2 matrices
    for(i = 0; i < rows1; i++){
        for(j = 0; j < columns2; j++)
            for(k = 0; k < columns1; k ++)
                mat3[i][j] += mat1[i][k] * mat2[k][j];
    }
    //Write the resulted matrix in the file
	fp3 = fopen("Matrix3.txt","w");
	//Dimensions of result matrix is rows1 * columns2
	for(i = 0; i < rows1; i++){
        for(j = 0; j < columns2; j++)
            fprintf(fp3, "%f ", mat3[i][j]);
		fprintf(fp3, "\n");
    }
	fclose(fp3);
    exit(EXIT_SUCCESS);
    return 0;
}
