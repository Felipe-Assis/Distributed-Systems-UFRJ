#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <cmath>
#include <semaphore.h>
#include <time.h>


using namespace std;

#define M 1000 //quantidade de números a serem gerados
#define T 10000000
#define I 0



//rotina simples para verificar se o número de entrada é primo
bool verificar(long int x){ 
	if (x<2){ return false; }
	for (int i=2;i < sqrt(x)+1; i++)
		if (x%i==0) {
			return false;
		}
	return true;	
}




//declaração de variáveis
long *vetor; //variável tipo (long) pois os número gerados variam de 1 até 10⁷
atomic <long> quant(M); //quantidade de números a serem gerados
int exibir = I;
sem_t cheio;
sem_t vazio;
sem_t run_thread;
mutex mtx1;
int NP;
int NC;
int N;



//======================
//Produtor
//======================
void *produtor(){
	while (true){
		if (quant < 1) break;
		sem_wait(&vazio);
		mtx1.lock();
		for (int i = 0; i < N; i++){
			if (vetor[i] == 0){
				vetor[i] = rand()%T+1;
				break;
			}
		}
		mtx1.unlock();
		sem_post(&cheio);		
	}
	sem_post(&vazio);
	sem_post(&cheio);
}


//======================
//Consumidor
//======================
void *consumidor(){

	long num; //dados extraídos do vetor de números são amarzenados localmente
	while (true){
		if (quant < 1) break;
		sem_wait(&cheio);
		mtx1.lock();
		for (int i = 0; i < N; i++){
			if (vetor[i] !=0){
				num = vetor[i];
				vetor[i] = 0; //limpa a i-ésima posição na memória
				quant.fetch_sub(1,memory_order_relaxed);
				break;
			}
		}	
		mtx1.unlock();
		sem_post(&vazio);

		verificar(num);
		// if (verificar(num)){
		// 	cout << num << " <Primo = TRUE>" << endl;
		// }
		// else
		// 	cout << num << " <Primo = FALSE>" << endl;

		}
	sem_post(&vazio);
	sem_post(&cheio);
	}

int main(int argc, char** argv){
	if (argc != 4){
		cout << "Erro!" << endl;
		return 0;
	}

	// cout << "Número de cores disponíveis: " << std::thread::hardware_concurrency() << endl;
	N = atoi(argv[1]);  //N = tamanho do vetor de int
    NP = atoi(argv[2]);  //P = quantidade de threads produtoras
    NC = atoi(argv[3]);  //C = quantidade de threads consumidoras

	thread threads [NP+NC];
	vetor = new long[N];
	srand (time(NULL));
	sem_init(&cheio, 0, 0);
	sem_init(&vazio, 0, N);

	for (int i =0; i< N; i++){ vetor[i] = 0;}
		
	struct timespec start, finish;
	double elapsed;		
	clock_gettime(CLOCK_MONOTONIC, &start);

	for (int i=0; i<NP+NC; i++){
		if (i < NP) 
			threads[i] = thread(produtor);
		else 
			threads[i] = thread(consumidor);
	}
	for (int i=0; i<NP+NC; i++){
	 	threads[i].join();}

	clock_gettime(CLOCK_MONOTONIC, &finish);

	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;


	mtx1.~mutex();
	sem_destroy(&cheio);
	sem_destroy(&vazio);
	delete vetor;

	cout << "Temp: " << elapsed << " segundos" << endl;

}
