#include <iostream>
#include <thread>
#include <atomic>
#include <time.h>
#include <algorithm>


using namespace std;

atomic_flag lock = ATOMIC_FLAG_INIT;



void acquire(){
	while(lock.test_and_set());
}

void release(){
	lock.clear();
}

char* vetor_num; //vetor de números
int N, K;
int resultado = 0;
int contador = 0;
bool pronto = false;

void *somador(long id){
	int aux = 0;
	long inicio = id*(N/K);
	long fim = inicio + (N/K);

	for (long i = inicio; i < fim; i++){
		aux += vetor_num[i];
	}

	acquire();
	resultado += aux; //soma o resultado da thread atual ao montante
	release();
}


int main(int argc, char** argv){
	if (argc != 3 || (atoi(argv[1])<atoi(argv[2]))){
		cout << "Erro!" << endl;
		return 0;
	}

	N = atoi(argv[1]);  //N = tamanho do vetor de int
    K = atoi(argv[2]); //K = número de threads
 
	srand(time(NULL));
	
	vetor_num = new char[N];
	thread threads[K];

	cout << endl << "==============================================" << endl;
	cout << "Inicializando vetor de números..." << endl;

	for (int i =0; i < N; i++){ 
		vetor_num[i] =  (char) (rand()%201 - 100); }
	cout << "Vetor inicializado." << endl;
	cout << "==============================================" << endl << endl;

	//Threads criadas
	struct timespec start, finish;
	double elapsed;		
	clock_gettime(CLOCK_MONOTONIC, &start);

	for (int i = 0; i<K; i++){
		threads[i] = thread(somador, i);
	}

	for (int i = 0; i < K; i++){
	 	threads[i].join();}

	clock_gettime(CLOCK_MONOTONIC, &finish);

	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

	cout << "Soma terminada." << endl;
	cout << "Temp: " << elapsed << " segundos" << endl;

	delete [] vetor_num;

    return 0;
}
