#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
/**
Implementation of a multi-threaded matrix multiplication program.
**/
/*int A[2][3] = {{1,2,3},{4,5,6}};
int B[3][2] = {{7,8},{9,10},{11,12}};
int c[2][2] ;
*/
float **A =NULL;
float **B =NULL;
float **C =NULL;
int m,t,n;
//Function contain main loops for matmult
void matrixmultperElement();
//Function to multiply elements for each thread
void *threadedMatmultPerElement();
//Function contain main loops for matmult
void matrixmultperRow();
//Function to multiply elements for each row
void *threadedMatmultPerRow();

//Function to build matrix
void buildMat();
//To pass row and column attributes to the thread
struct eleposition {
    int row;
    int column;
};
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
    m = rows1;
    t = columns1;
    n = columns2;
    A = (float **)malloc(m * sizeof(float*));
    int x = 0,z = 0;
    //allocating columns memory
    for (x = 0; x < m; x++) {
        A[x] = (float *)malloc(t* sizeof(float));
    }
    //copying from mat1 to A
    for (x = 0; x < m; x++) {
        for (z = 0; z < t; z++)
            A[x][z] = mat1[x][z];
    }
    printf("A\n");
    for (x = 0; x < m; x++) {
        for (z = 0; z < t; z++) {
            printf("%f ", A[x][z] );
        }
        printf("\n");
    }
    printf("\n");
//*****************************************************************************
    //allocating rows memory
    B= (float **)malloc(t * sizeof(float*));
    //allocating columns memory
    for (x = 0; x < t; x++) {
        B[x]=(float *)malloc(n* sizeof(float));
    }
    //copying from mat2 to B
    for (x = 0; x < t; x++) {
        for (z = 0; z < n; z++)
            B[x][z] = mat2[x][z];
    }
    printf("B\n");
    for (x = 0; x < t; x++) {
        for (z = 0; z < n; z++) {
            printf("%f ", B[x][z] );
        }
        printf("\n");
    }
    printf("\n");
//*****************************************************************************
    C= (float **)malloc(m * sizeof(float*));
    //allocating columns memory
    for (x = 0; x < m; x++) {
        C[x]=(float *)malloc(n* sizeof(float));
    }
    //Element threading
    matrixmultperElement();
    printArray();
    printf("\n");
//*****************************************************************************
    C= (float **)malloc(m * sizeof(float*));
    //allocating columns memory
    for (x = 0; x < m; x++) {
        C[x] = (float *)malloc(n* sizeof(float));
    }
    //Row threading
    matrixmultperRow();
    printArray();
    // MulwithoutThreads();
    exit(EXIT_SUCCESS);
    return 0;
}
void matrixmultperElement() {
    //calculating number of elements in c to creat threads
    int size = sizeof(C)/sizeof(C[0][0]);
    //array of threads
    pthread_t tid[size];
    pthread_attr_t attr ;
    pthread_attr_init(&attr);
    //threads itrator
    int q = 0;
    int i, j;
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            struct eleposition *ptr ;
            ptr = (struct eleposition*)malloc(sizeof(struct eleposition));
            ptr->row = i;
            ptr->column = j;
            pthread_create(&tid[q],&attr,threadedMatmultPerElement,ptr);
            q++;
        }
    }
    //waiting for all threads to finish
    int z;
    for (z = 0; z < size; z++) {
        pthread_join(tid[z],NULL);
    }
}
void *threadedMatmultPerElement (void *ptr) {
    struct eleposition *pos ;
    pos= ptr;
    int i;
    float result=0;
    for (i=0; i<t; i++) {
        result+=  A[pos->row][i] * B[i][pos->column];
    }
    C[pos->row][pos->column] = result;
    pthread_exit(0);
}
void matrixmultperRow() {
    //array of threads
    pthread_t tid[m];
    pthread_attr_t attr ;
    pthread_attr_init(&attr);
    //threads itrator
    int i;
    for (i = 0; i < m; i++) {
        pthread_create(&tid[i],&attr,threadedMatmultPerRow,i);
    }
    //waiting for all threads to finish
    int z;
    for (z = 0; z < m ; z++) {
        pthread_join(tid[z],NULL);
    }
}
void *threadedMatmultPerRow (int r) {
    int i = r;
    int j, k;
    float result=0;
    for(j = 0; j < n; j++)
            for(k = 0; k < t; k ++)
                C[i][j] += A[i][k] * B[k][j];
    pthread_exit(0);
}
void printArray() {
    FILE *fp3;
    int i,j;
    fp3 = fopen("Matrix3.txt","w");
	//Dimensions of result matrix is rows1 * columns2
	for(i = 0; i < m; i++){
        for(j = 0; j < n; j++)
            fprintf(fp3, "%f ", C[i][j]);
		fprintf(fp3, "\n");
    }
	fclose(fp3);
}
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

