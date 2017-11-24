#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


/*int A[2][3] = {{1,2,3},{4,5,6}};
int B[3][2] = {{7,8},{9,10},{11,12}};
int c[2][2] ;
*/

float **A =NULL;
float **B =NULL;
float **C =NULL;

int m,t,n;


// function contain main loops for matmult
void matrixmultperElement();
// function to multiply elements for each thread
void *threadedMatmultPerElement ();
void buildMat();

struct eleposition
{
    int row;
    int column;
};

int main()
{


    FILE * fp1, * fp2;
    char * line1 = NULL, * line2 = NULL;
    float mat1[10][10], mat2[10][10], mat3[10][10];
    int i = 0, j = 0, k = 0;
    int rows1 = 0, columns1 = 0, rows2 = 0, columns2 = 0;
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
    for(i = 0; i < rows1; i++){
        for(j = 0; j < columns1 ; j++)
            printf("%f ", mat1[i][j]);
        printf("\n");
    }

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
    for(i = 0; i < rows2; i++){
        for(j = 0; j < columns2; j++)
            printf("%f ", mat2[i][j]);
        printf("\n");
    }
    for(i = 0; i < rows1; i++){
        for(j = 0; j < columns1; j++)
            mat3[i][j] = 0;
    }
    for(i = 0; i < rows1; i++){
        for(j = 0; j < columns2; j++)
            for(k = 0; k < columns1; k ++)
                mat3[i][j] += mat1[i][k] * mat2[k][j];
    }
	for(i = 0; i < rows1; i++){
        for(j = 0; j < columns2; j++)
            printf("%f ", mat3[i][j]);
		printf("\n");
    }


    m=rows1;
    t=columns1;
    n=columns2;

   // printf("m=%d t=%d n=%d  ",m,t,n);

    A= (float **)malloc(m * sizeof(float*));
    int x=0,z=0;
    //allocating columns memory
    for (x=0; x<m; x++)
    {
        A[x]=(float *)malloc(t* sizeof(float));
    }


    //copying from mat1 to A
    for (x=0; x<m; x++)
    {
        for (z=0; z<t; z++)
            A[x][z]=mat1[x][z];
    }


    printf("A\n");
    for (x=0; x<m; x++)
    {
        for (z=0; z<t; z++)
        {
            printf("%f ", A[x][z] );
        }
        printf("\n");

    }
//*****************************************************************************
    //allocating rows memory
    B= (float **)malloc(t * sizeof(float*));
    //allocating columns memory
    for (x=0; x<t; x++)
    {
        B[x]=(float *)malloc(n* sizeof(float));
    }


    //copying from mat2 to B
    for (x=0; x<t; x++)
    {
        for (z=0; z<n; z++)
            B[x][z]=mat2[x][z];
    }


    printf("B\n");
    for (x=0; x<t; x++)
    {
        for (z=0; z<n; z++)
        {
            printf("%f ", B[x][z] );
        }
        printf("\n");

    }

//*****************************************************************************
    C= (float **)malloc(m * sizeof(float*));
    //allocating columns memory
    for (x=0; x<m; x++)
    {
        C[x]=(float *)malloc(n* sizeof(float));
    }

    matrixmultperElement();
    printArray();
    // MulwithoutThreads();
    exit(EXIT_SUCCESS);
    return 0;
}

void matrixmultperElement()
{

    // calculating number of elements in c to creat threads
    int size = sizeof(C)/sizeof(C[0][0]);

    //array of threads
    pthread_t tid[size];
    pthread_attr_t attr ;
    pthread_attr_init(&attr);

    //threads itrator
    int q=0;

    int i,j;
    for (i=0; i<m; i++)
    {
        for (j=0; j<n; j++)
        {
            struct eleposition *ptr ;
            ptr = (struct eleposition*)malloc(sizeof(struct eleposition));
            ptr->row = i;
            ptr->column = j;
            pthread_create(&tid[q],&attr,threadedMatmultPerElement,ptr);
            q++;

        }

    }

    // waiting for all threads to finish
    int z;
    for (z=0; z<size ; z++)
    {
        pthread_join(tid[z],NULL);

    }
    //printf("k=%d \n",k);
}

void *threadedMatmultPerElement (void *ptr)
{

    // ask leh mknsh bysht8l mn 8er da?!!!
    struct eleposition *pos ;
    pos= ptr;
    int i;
    float result=0;
   // printf("here k= %d",t);
    for (i=0; i<t; i++)
    {
        result+=  A[pos->row][i] * B[i][pos->column];
       // printf("---->%d",result);
    }
    C[pos->row][pos->column] = result;
    //printf("result  c= %f\n",result);

    pthread_exit(0);
}

void printArray()
{

    int i,j;
    for (i=0; i<m; i++)
    {
        for (j=0; j<n; j++)
        {
            printf("%f\t",C[i][j]);
        }
        printf("\n");
    }

}

void buildMat(float mat[][10], int i, int j, char *line, int *columns) {

    int iline = 0, jj = 0, col;
    char tempchar[j];
    float tempint = 0;
    for(col = 0; col < j; col++) {
		if(line[col] == ' ' || col == j-1) {
			if(tempchar != NULL) {
				tempint = atof(tempchar);
				mat[i][jj++] = tempint;
			}
			iline = 0;
		}
		else
    		tempchar[iline++] = line[col];
    	}
    *columns = jj;
}

