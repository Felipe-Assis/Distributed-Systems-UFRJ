#include <iostream>
#include <thread>
#include <vector>


using namespace std;
int K, N;


//Pass number of threads K and amount of numbers 10^N as argument
int main(int argc, char** argv){
	if (argc != 3){
		cout << "Erro! Número inválido de argumentos!" << endl;
		cout << "K = número de threads" << endl;
		cout << "N = tamanho do vetor de int" << endl;
		return 0;
}

	K = atoi(argv[1]); //K = número de threads
	N = atoi(argv[2]); //N = tamanho do vetor de int



}