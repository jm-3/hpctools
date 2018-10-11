// Ejercicio realizado por José M. Martín


#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

double t1,t2,t3,t4;
double doublesperline;

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
	double aux, *kk;	
	clock_t tStart,tEnd;


        kk=(double*)malloc(sizeof(double)*n);
	// realizar la descomposicion LU
	//
	// mediante el algoritmo de Crout
	tStart=clock();
	for(j=0; j<n; j++){
		// calculo de betas correspondientes a U
		for(i=0;i<(j+1);i++){
			aux = a[i*n+j];
			for(k=0;k<i;k++)
				aux-=a[i*n+k]*a[k*n+j];
			a[i*n+j]=aux;
		}
	}
	tEnd=clock();
	t1+=tEnd-tStart;

	tStart=clock();
	for(j=0; j<n; j++){
		// calculo de alfas correspondientes a L
		for(i=j+1;i<n;i++){
			aux = a[i*n+j];
			for(k=0;k<j;k++)
				aux-=a[i*n+k]*a[k*n+j];
			a[i*n+j] = 1/a[j*n+j] * aux; 
		}
	}
	tEnd=clock();
	t2+=tEnd-tStart;

	
	// para cada columna de b
	tStart=clock();
        for(l=0;l<n;l++){
            // copiar 
            for(i=0;i<n;i++){
                kk[i]=b[i*n+l];
            }            
            // resolver Ly=b
            for(i=0;i<n;i++){
		aux=kk[i];
		for(j=0;j<i;j++)
			aux-=a[i*n+j]*kk[j];
		kk[i]=aux;
            }
            for(i=0;i<n;i++){
                b[i*n+l]=kk[i];
            }            
            
        }
	tEnd=clock();
	t3+=tEnd-tStart;

	tStart=clock();
	for(l=0;l<n;l++){	
            // resolver Ux=y		
            // copiar 
            for(i=0;i<n;i++){
                kk[i]=b[i*n+l];
            }            
            for(i=n-1;i>-1;i--){
                    aux=kk[i];
                    for(j=i+1;j<n;j++)
                            aux-=a[i*n+j]*kk[j];
                    kk[i]=aux/a[i*n+i];
            }
            for(i=0;i<n;i++){
                b[i*n+l]=kk[i];
            }            		
	}
	tEnd=clock();
	t4+=tEnd-tStart;
    
}

int check_solution(int size, double *a){
    int i,j,counter;
    double umbral=1e-8, expected_value;
    counter=0;
    for(i=0;i<size;i++)
	for(j=0;j<size;j++) {
	if(counter==(size*i)+j)
		expected_value=1.0;
	else
		expected_value=0.0;
        if (fabs(expected_value-a[i*size+j])>umbral) { printf("%d %d %lf\n",i,j,a[i*size+j]);return 0;}
        counter++;
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
        int linesize;
	
	t1=t2=t3=t4=0;
        
        linesize=sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
        doublesperline=linesize/sizeof(double);

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
	
		printf("Tiempo de los bucles:\n\tb1: %.3lf\n\tb2: %.3lf\n\tb3: %.3lf\n\tb4: %.3lf\n",t1/CLOCKS_PER_SEC,t2/CLOCKS_PER_SEC,t3/CLOCKS_PER_SEC,t4/CLOCKS_PER_SEC);

        printf("%s %s: time taken by my implementation: %.3fs\n", argv[0], argv[1], (double) (tEnd - tStart) / CLOCKS_PER_SEC);

    
        return 0; 
    }
