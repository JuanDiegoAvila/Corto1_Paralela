#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Universidad del Valle de Guatemala - Corto 1 - Computacion paralela y distribuida
// Juan Diego Avila 20090

// Calculo de fibonacci de forma recursiva
int fib_recursive(int n) {
    if (n <= 1) { // fibonacci de 0 es 0 y 1 de es 1
        return n;
    }
    return fib_recursive(n - 1) + fib_recursive(n - 2); // fibonacci de n es la suma de los dos anteriores. llamadas recursivas
}

// Calculo de fibonacci de forma recursiva pero con la directiva task de openmp
int fib_recursive_omp(int n){
    if (n <= 1) {
        return n;
    }
    int x = 0;
    int y = 0;
    int result = 0;

    #pragma omp task shared(x)
    x = fib_recursive_omp(n - 1);

    #pragma omp task shared(y)
    y = fib_recursive_omp(n - 2);

    #pragma omp taskwait
    result = x + y;

    return result;
}

// Calculo de fibonacci de forma recursiva pero con la directiva task de openmp pero con threshold
int fib_recursive_omp_fix(int n){
    int THRESHOLD = 15; // se establece el threshold
    
    if (n <= 1) {
        return n;
    }
    int x = 0;
    int y = 0;
    int result = 0;

   
    if (n <= THRESHOLD){

        x = fib_recursive_omp_fix(n - 1);

    }else{
        #pragma omp task shared(x)
        x = fib_recursive_omp_fix(n - 1);

    }
   
    if (n <= THRESHOLD){
        y = fib_recursive_omp_fix(n - 2);
    }
    else{
        #pragma omp task shared(y)
        y = fib_recursive_omp_fix(n - 2);

    }

    if(n > THRESHOLD)
    {
        #pragma omp taskwait
        result = x + y;
    }
    else{
        result = x + y;
    }

    return result;
}
 
int main(int argc, char* argv[]) {
    int n = atoi(argv[1]); // se obtiene el numero de fibonacci a calcular

    omp_set_num_threads(10); // se establece el numero de hilos a usar

    int fibonacci = 0;

    #pragma omp parallel
    {
        fibonacci = fib_recursive_omp_fix(n); // se llama a la funcion 
       
    }
    printf("Fibonacci de %d es %d\n", n, fibonacci);
    
        
    return 0;
    
}