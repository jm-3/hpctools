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

void my_dgesv(int n, double *a, double *b) {
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
	}

	for(j=0; j<n; j++)
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
	}

	for(l=0;l<n;l++){	
		// resolver Ux=y
		for(i=n-1;i>-1;i--){
			aux=b[i*n+l];
			for(j=i+1;j<n;j++)
				aux-=a[i*n+j]*b[j*n+l];
			b[i*n+l]=aux/a[i*n+i];
		}
	}
    
}

int check_solution(int size, double *a){
    int i,j,counter;
    double umbral=1e-8, expected_value;
    counter=0;
    for(i=0;i<size;i++)
	for(j=0;j<size;j++) {
	if(counter==(size*i)+i)
		expected_value=1.0;
	else
		expected_value=0.0;
        if (fabs(expected_value-a[i])>umbral) return 0;
    }
    return 1;

}


int main(int argc, char *argv[])
    {
	if(argc<2){
		printf("Error: tiene que indicar el tamaño de la matriz\n");
		return 0;
	}

        int size = atoi(argv[1]);

        double *a;
        double *b;
	clock_t tStart,tEnd;

        a = generate_matrix(size);
        b = generate_matrix(size);
        

        tStart = clock();   
        my_dgesv(size, a, b);
	tEnd = clock();

	if(check_solution(size,b)){
		printf("Solución correcta!!\n");
	}else{
		printf("Solución incorrecta!!!\n");
	}

        printf("%s %s: time taken by my implementation: %.3fs\n", argv[0], argv[1], (double) (tEnd - tStart) / CLOCKS_PER_SEC);

    
        return 0; 
    }
