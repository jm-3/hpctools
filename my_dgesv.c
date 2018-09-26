// Ejercicio realizado por José M. Martín


#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double *generate_matrix(int size)
{
    int i;
    double *matrix = (double *)malloc(sizeof(double) * size * size);
    srand(1);

    for (i = 0; i < size * size; i++)
    {
        matrix[i] = rand() % 100;
    }

    return matrix;
}

void print_matrix(const char *name, double *matrix, int size)
{
    int i, j;
    printf("matrix: %s \n", name);

    for (i = 0; i < size; i++)
    {
            for (j = 0; j < size; j++)
            {
                printf("%f ", matrix[i * size + j]);
            }
            printf("\n");
    }
}

int my_dgesv(int n, double *a, double *b) {
	int i,j,k,l;
	double aux;


	// realizar la descomposicion LU
	//
	// mediante el algoritmo de Crout
	for(j=0; j<n; j++){
		// calculo de betas correspondientes a U
		for(i=0;i<(j+1);i++){
			aux = a[i*n+j];
			for(k=0;k<i;k++)
				aux-=a[i*n+k]*a[k*n+j];
			a[i*n+j]=aux;
		}
		// calculo de alfas correspondientes a L
		for(i=j+1;i<n;i++){
			aux = a[i*n+j];
			for(k=0;k<j;k++)
				aux-=a[i*n+k]*a[k*n+j];
			a[i*n+j] = 1/a[j*n+j] * aux; 
		}
	}

	
	// para cada columna de b
	for(l=0;l<n;l++){
		// resolver Ly=b
		for(i=0;i<n;i++){
			aux=b[i*n+l];
			for(j=0;j<i;j++)
				aux-=a[i*n+j]*b[j*n+l];
			b[i*n+l]=aux;

		}
	
		// resolver Ux=y
		for(i=n-1;i>-1;i--){
			aux=b[i*n+l];
			for(j=i+1;j<n;j++)
				aux-=a[i*n+j]*b[j*n+l];
			b[i*n+l]=aux/a[i*n+i];
		}
	}
    
}


    void main(int argc, char *argv[])
    {

        int size = atoi(argv[1]);

        double *a;
        double *b;
	clock_t tStart;

        a = generate_matrix(size);
        b = generate_matrix(size);
        

        tStart = clock();   
        my_dgesv(size, a, b);
        printf("Time taken by my implementation: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
        
        
    }
