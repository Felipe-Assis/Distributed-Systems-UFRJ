#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <iostream>
#include <limits>
#include <unistd.h>
#include <string>
#include <cstdlib>
#include <ctime>

#define TAMANHO 20

using namespace std;



string wait_type; // global por causa de uso em main() fora do escopo de Wait_Config()

void Wait_Config()
{
	if (wait_type == "Busy")
    {
        printf("RECEPTOR: BUSY WAIT\n");
		while(1){
        	sleep(1);
    	}
    }
    else if (wait_type == "Blocking")
    {
        printf("RECEPTOR: BLOCKING WAIT\n");
    	while(1){
        	pause();
    	}
    }
}



void Signal_Handler(int iID)
{
    if(iID == SIGUSR1){
        printf("RECEPTOR: SIGUSR1 Recebido\n");
    }        
    else if(iID == SIGUSR2){
        printf("RECEPTOR: SIGUSR2 Recebido\n");
    }       
    else if(iID == SIGINT){
        printf("RECEPTOR: SIGINT Recebido\n");
    }  
}

bool verificar(int x){ //rotina simples para verificar se o número de entrada é primo
	if (x==1){ return false; }
	for (int i=2;i < (x/2+1);i++)
		if (x%i==0) {
			return false;
		}
	return true;	
}




int main(int argc, char **argv)
{
    printf("--programa iniciado--\n");

    string work_type; //muda entre pipes e signals


	int num;//quantidade de números a serem produzidos e enviados
	int conexao[2];//
	pipe(conexao);//    
	int temp = 0; //variável temporária usada na geração de números crescentes
	int fator; //fator de cresciment da sequência de números geradas
	int x; //guarda o número recebido através do pipe depois da conversão de string para int


    printf("Escolha o trabalho: Sinais ou Pipes\n");
    printf("Entre com o trabalho desejado: ");
    getline(cin, work_type);
    while (work_type != "Sinais" and work_type != "Pipes")
    {
		printf("Apenas Sinais ou Pipes!\n");
		printf("Entre novamente com o tipo de wait: ");
	    getline(cin, work_type);
	}

	if (work_type == "Sinais")
	{
		printf("Escolha o tipo de wait: Busy ou Blocking\n");
		printf("Entre com o tipo de wait: ");
		getline(cin, wait_type);
		while (wait_type != "Busy" and wait_type != "Blocking")
		{
			printf("Apenas Busy ou Blocking!\n");
			printf("Entre novamente com o tipo de wait: ");
		    getline(cin, wait_type);
		}
	}

    pid_t pid = fork(); //FUNCIONAMENTO DO FORK: variável pid é o Process ID do PROCESSO FILHO, MAS QUE O PROCESSO PAI RECEBE, pid no processo filho é setada em 0!!!!!!

    if (pid == 0)
    	//PROCESSO FILHO = RECEPTOR
    {
    	if (work_type == "Sinais")
    	{
	        signal(SIGUSR1, Signal_Handler);
	    	signal(SIGUSR2, Signal_Handler);
	    	signal(SIGINT, Signal_Handler);
	    	Wait_Config();
    	}


    	else if (work_type == "Pipes")
    	{
			char s_num[TAMANHO]; //variável para receber o conteúdo da leitura	
			close(conexao[1]); //fecha a escrita do processo filho
			read(conexao[0], s_num, TAMANHO); //lê a mensagem do processo pai e atribui o conteúdo recebido a variável s_num

			cout << endl;
			cout << "===========================================" << endl;
			cout << "         Consumidor inicializado" << endl;
			cout << "===========================================" << endl;
			cout << endl;

			while (stoi(s_num) > 0){
				x = stoi(s_num); //converte string para int de modo a utilizar a rotina para verificar se o número é primo

				if (verificar(x)){ cout << "Consumidor: " << x << " é primo" << endl;}
				else { cout << "Consumidor: " << x << " não é primo" << endl;}

				read(conexao[0], s_num, TAMANHO); //repete a leitura para cada número subsequente
			}
			close(conexao[0]); //fecha a leitura do processo filho
    	}

    }

    else if (pid > 0)
    {
        //PROCESSO PAI = EMISSOR

		string input_signal; //compartilhada por modos manual e automatico (USR1, USR2, INT ou KILL)

		while (1) //o grande imenso infinito majestoso menu
		{
			if (work_type == "Sinais")
			{
				string pid_input_type; //modos manual ou automatico

			    printf("\nEscolha o tipo de entrada de Process ID: Manual ou Automatico (sempre envia ao receptor)\n");
				sleep(1);
				printf("Entre com o tipo de entrada de Process ID: ");
				getline(cin, pid_input_type);


				while (pid_input_type != "Manual" and pid_input_type != "Automatico")
				{
					printf("\nApenas Manual ou Automatico!\n");
					printf("Entre novamente com o tipo de entrada de Process ID: ");
	    			getline(cin, pid_input_type);
				}


				if (pid_input_type == "Manual")
				{
					printf("\nO Process ID do Receptor e %d\n", pid);
					printf("\nEscolha o sinal a ser enviado: USR1, USR2, INT ou KILL (nao ha opcao de escolha de Process ID de envio para o sinal KILL e este encerra ambos os processos)\n");
					//sleep(1);
				    printf("Entre com o sinal: ");
				    getline(cin, input_signal);

				    while (input_signal != "USR1" and input_signal != "USR2" and input_signal != "INT" and input_signal != "KILL")
				    {
				    	printf("Sinal nao suportado\n");
				    	printf("Entre novamente com um dos sinais supracitados: ");
					    getline(cin, input_signal);
				    }

				    if (input_signal == "KILL")
				    {
		                kill(pid, SIGKILL);
		                printf("\nEMISSOR: SIGKILL Emitido, terminando processos\n");
		                break;
				    }

				    else
			    	{
					    int manual_pid_input; //podendo ser o do receptor ou nao, com a excecao de kill por motivos de potencial kernel failure

					    printf("\nEntre com o Process ID desejado (apenas numeros): ");
						cin >> manual_pid_input;


						//CHECAR SE PROCESSO EXISTE, CIN ERROR HANDLER
			    		while(!(cin >> manual_pid_input) or kill(manual_pid_input, 0) != 0)
			    		{
						    cin.clear();
						    cin.ignore(numeric_limits<streamsize>::max(), '\n');
						    cout << "Process ID nao encontrado ou entrada invalida (com letras, por exemplo). Tente novamente: ";
						}



					    if (input_signal == "USR1")
					    {
			                kill(manual_pid_input, SIGUSR1);
			                printf("\nEMISSOR: SIGUSR1 Emitido ao Process ID %d\n", manual_pid_input);
					    }
					    else if (input_signal == "USR2")
					    {
			                kill(manual_pid_input, SIGUSR2);
			                printf("\nEMISSOR: SIGUSR2 Emitido ao Process ID %d\n", manual_pid_input);
					    }
					    else if (input_signal == "INT")
					    {
			 			    kill(manual_pid_input, SIGINT);
			                printf("\nEMISSOR: SIGINT Emitido ao Process ID %d\n", manual_pid_input);
					    }
					    else
					    {
					    	printf("\nApenas os comandos supracitados sao aceitos\n");
					    }

					}

				}
				if (pid_input_type == "Automatico")
				{
		    		printf("\nEscolha o sinal a ser enviado: USR1, USR2, INT ou KILL (termina ambos os processos)\n");
					//sleep(1);
				    printf("Entre com o sinal: ");
				    getline(cin, input_signal);


				    if (input_signal == "USR1")
				    {
		                kill(pid, SIGUSR1);
		                printf("\nEMISSOR: SIGUSR1 Emitido\n");
				    }
				    else if (input_signal == "USR2")
				    {
		                kill(pid, SIGUSR2);
		                printf("\nEMISSOR: SIGUSR2 Emitido\n");
				    }
				    else if (input_signal == "INT")
				    {
		 			    kill(pid, SIGINT);
		                printf("\nEMISSOR: SIGINT Emitido\n");
				    }
				    else if (input_signal == "KILL")
				    {
		                kill(pid, SIGKILL);
		                printf("\nEMISSOR: SIGKILL Emitido, terminando processos\n");
		                break;
				    }
				    else
				    {
				    	printf("\nApenas os comandos supracitados sao aceitos\n");
				    }
			    }
			}
			else if (work_type == "Pipes")
			{
				srand(time(NULL));
				cout << "==========================================" << endl;
				cout << "           Produtor inicializado" << endl;
				cout << "==========================================" << endl;
				cout << "Insira a quantidade de números a serem gerados: ";
				cin >> num;
				cout << "Insira o fator de crescimento da sequência de números: ";
				cin >> fator;
				cout << endl;


				string s_num; //o número será enviado através do pipe como uma string

				close(conexao[0]); //fecha leitura do processo pai
				for (int i=0; i<num; i++){
					temp += rand()%fator+1; //gera números "aleatórios" crescentes
					cout << "Produtor: " << temp << endl;
					s_num=to_string(temp); //converte o número guardado na variável <temp> para string e atribui este valor a s_num
					write(conexao[1], s_num.c_str(), TAMANHO); //
				}

				cout << "Todos os números foram enviados" << endl;
				s_num = to_string(0); // 0 é enviado para sinalizar o fim da sequência
				write(conexao[1], s_num.c_str(), TAMANHO);
				close(conexao[1]); //fecha escrita do processo filho
				exit(0);
			}		
		}
    }

    else
    {
        // fork failed
        printf("fork() falhou!\n");
        return 1;
    }

    printf("--programa terminado--\n");

    return 0;
}

