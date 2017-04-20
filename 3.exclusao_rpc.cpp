#include <iostream>
#include <thread>
#include <vector>


using namespace std;
//Declaration of variables
int T, K, X, Y;



//Pass number of threads K and amount of numbers 10^N as argument
int main(int argc, char** argv){
	if (argc != 5){
		cout << "Erro! Número inválido de argumentos!" << endl;
		cout << "T: limite superior do tempo" << endl;
		cout << "K: número de threads" << endl;
		cout << "X: número de frases a serem escritas" << endl;
		cout << "Y: intervalo entre processos" << endl;
		return 0;
}

	T = atoi(argv[1]); //T: limite superior do tempo
	K = atoi(argv[2]); //K: número de threads
	X = atoi(argv[3]); //X: número de frases a serem escritas
	Y = atoi(argv[4]); //Y: intervalo entre processos

	

}