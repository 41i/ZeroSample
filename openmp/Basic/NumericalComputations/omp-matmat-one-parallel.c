/*****************************************************************************




 Example               : omp-matmat-one-parallel.c

 Objective             : Matrix - Matrix Multiplication using

                  	 OpenMP one PARALLEL for directive and Private Clause

 
 Input                 : Size of Matrices(i.e Size of Matrix A and Matrix B) ie in terms of

                  	 CLASS where CLASS A :1024; CLASS B: 2048 and CLASS C: 4096

                 	 Number of Threads


 Output                : Number of Threads

                 	 Total Memory Utilized for the Matrix - Matrix Computation

                  	 Total Time Taken for Matrix - Matrix Computaion
	                                            
                                                                        
 Created               : Aug 2011
 Author                : RarchK                                          


*********************************************************************************/


#include <stdio.h>
#include <sys/time.h>
#include <omp.h>
#include<string.h>
#include <stdlib.h>


/*              Function declaration            */ 

double Matrix_Multiplication_One(double **Matrix_A,double **Matrix_B,double **Result,int N_size,int Total_threads);


/* Main Program */

main(int  argc , char * argv[])
{
	int             CLASS_SIZE,N_size, i,j,k,Total_threads,THREADS;
	double          Total_overhead = 0.0;
	double         **Matrix_A, **Matrix_B, **Result;
        double           memoryused=0.0;
        int             iteration;
 	FILE            *fp;
        char            *CLASS;

        
        printf("\n\t\t---------------------------------------------------------------------------");
        printf("\n\t\t Email : RarchK");
        printf("\n\t\t---------------------------------------------------------------------------");
        printf("\n\t\t Objective : Dense Matrix Computations (Floating Point Operations)\n ");
        printf("\n\t\t Matrix into Matrix Multiplication using ");
        printf("\n\t\t OpenMP one PARALLEL for directive and Private Clause;");
        printf("\n\t\t..........................................................................\n");

         /* Checking for command line arguments */
        if( argc != 3 ){
       
           printf("\t\t Very Few Arguments\n ");
           printf("\t\t Syntax : exec <Class-Size> <Threads>\n");
           printf("\t\t Where : Class-Size = A or B or C\n");
           exit(-1);
        }
        else {
           CLASS = argv[1];
 	   THREADS = atoi(argv[2]);
        }
       if( strcmp(CLASS, "A" )==0){
            CLASS_SIZE = 1024;
       }
       if( strcmp(CLASS, "B" )==0){
            CLASS_SIZE = 2048;
       }
       if( strcmp(CLASS, "C" )==0){
            CLASS_SIZE = 4096;
       }



        N_size = CLASS_SIZE;
        Total_threads = THREADS;   		
        printf("\n\t\t Matrix Size :  %d",N_size);
        printf("\n\t\t Threads     :  %d",Total_threads);
        printf("\n");
	
	/* Matrix_A Elements */
	Matrix_A = (double **) malloc(sizeof(double *) * N_size);
	for (i = 0; i < N_size; i++) {
		Matrix_A[i] = (double *) malloc(sizeof(double) * N_size);
		for (j = 0; j < N_size; j++)
		{
		//	srand48((unsigned int)N_size);
		//	Matrix_A[i][j] = (double)(rand()%10);
			Matrix_A[i][j] = i+j;
		}
			
	}

	/* Matrix_B Elements */
	Matrix_B = (double **) malloc(sizeof(double *) * N_size);
	for (i = 0; i < N_size; i++) {
		Matrix_B[i] = (double *) malloc(sizeof(double) * N_size);
		for (j = 0; j < N_size; j++)
		{
	        //	 srand48((unsigned int)N_size);
                  //      Matrix_B[i][j] = (double)(rand()%10);
			Matrix_B[i][j] = i+j;
		}
	
	}

	/* Dynamic Memory Allocation */
	Result = (double **) malloc(sizeof(double *) * N_size);

	for (i = 0; i < N_size; i++) 
		Result[i] = (double *) malloc(sizeof(double) * N_size);

        memoryused = (3*(N_size*N_size))*sizeof(double);
	
	/* Function Calling   */
       Total_overhead = Matrix_Multiplication_One(Matrix_A,Matrix_B,Result,N_size,Total_threads);


       printf("\n\t\t Memory Utilized            : %lf MB \n",(memoryused/(1024*1024)));
       printf("\n\t\t Time in Seconds (T)        : %lf Seconds \n",Total_overhead);
       printf("\n\t\t   ( T represents the Time taken for the computation )");
       printf("\n\t\t..........................................................................\n");


	
	/* Free Memory     */
	free(Matrix_A);
	free(Matrix_B);
	free(Result);


}/* Main function end    */

/* Functions implementation   */
double Matrix_Multiplication_One(double **Matrix_A,double **Matrix_B,double **Result,int N_size,int Total_threads)
{
	int             i,j,k;
	struct timeval  TimeValue_Start;
	struct timezone TimeZone_Start;

	struct timeval  TimeValue_Final;
	struct timezone TimeZone_Final;
	long            time_start, time_end;
	double          time_overhead;
	
	gettimeofday(&TimeValue_Start, &TimeZone_Start);

        /* Set the no. of threads */
	omp_set_num_threads(Total_threads);

	/* OpenMP Parallel For Directive : Fork a team of threads giving them their own copies of variables */
	#pragma omp parallel for private(j,k)
	for (i = 0; i < N_size; i = i + 1)
		for (j = 0; j < N_size; j = j + 1){
			Result[i][j]=0.0;
			for (k = 0; k < N_size; k = k + 1)
				Result[i][j] = Result[i][j] + Matrix_A[i][k] * Matrix_B[k][j];
		}/* All threads join master thread and disband */

	gettimeofday(&TimeValue_Final, &TimeZone_Final);

	time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
	time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
	time_overhead = (time_end - time_start)/1000000.0;

        printf("\n\t\t Matrix into Matrix Multiplication using one Parallel for pragma......Done \n");

	
	return time_overhead;
}

