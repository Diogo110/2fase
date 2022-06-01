#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <errno.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "header.h"


jobList *newJobNode(int key) 
{
	jobList *node = (jobList *) malloc(sizeof(jobList));

	if(node == NULL) 
	{
		system("cls");
		printf("Nao foi possivel criar um no ou ramo\n\n");
		system("pause");
		return NULL;
	}
	else 
	{
        node->id = key;
        node->nOperations = 0;
        node->height = 0;
		node->opL = newOperationNode();
		node->next = NULL;
		return node;
	}
}
operationList *newOperationNode() 
{
	operationList *p = (operationList *) malloc( sizeof(operationList));
	
	if(p == NULL) 
	{
		system("cls");
		printf("Nao foi possivel criar a lista""\n\n");
		system("pause");
		return NULL;
	}
	else 
	{
		p->id = 0;
        p->quantMachines = 0;
        p->machineAndTime = NULL;
		p->next = NULL;
		return p;
	}
}

void listOperationNode(operationList *op) 
{
	int j = 0;

	printf("Id - (%d)\n",op->id);
	printf("Maquina - (");
	for (j = 0; j < op->quantMachines; ++j) 
	{
		if((op->quantMachines - j) == 1)
			printf("%d",op->machineAndTime[0*op->quantMachines + j]);
		else
			printf("%d,",op->machineAndTime[0*op->quantMachines + j]);
	}
	printf(")\nTempo - [");
	for (j = 0; j < op->quantMachines; ++j) 
	{
		if((op->quantMachines - j) == 1)
			printf("%d",op->machineAndTime[1*op->quantMachines + j]);
		else
			printf("%d,",op->machineAndTime[1*op->quantMachines + j]);
	}
	printf("]\n\n");

}

void listJobNode(operationList *auxOpL, jobList *jb) 
{
	int i = 0;

	for(i = 0; i < 25; i++) printf("%c",205);
	printf("\nJOB N%c %d\n",167,jb->id);

	if(jb->nOperations > 0) 
	{
		printf("Numero de operacoes: %d\n",jb->nOperations);
		while(auxOpL->next != NULL) {
            listOperationNode(auxOpL);
            auxOpL = auxOpL->next;
        }
	}
	else {
		printf("O job nao tem operacoes\n\n");
	}
}

void ListJobInOrder(jobList *n) 
{
    while(n->next != NULL) 
	{
		listJobNode(n->opL,n);
		n = n->next;
	}
}

int searchEqualMachine(operationList *m, int elemToFind, int currentPosition) 
{
	for(int i = currentPosition - 1; i >= 0; i--) 
	{
		if(m->machineAndTime[i] == m->machineAndTime[currentPosition])
			return 0;
	}

	return 1;
}

void createOperationFromFile(operationList *opL, int idOp, int nMachines, int arrM[], int arrT[]) 
{
	operationList *pOpL;
	int i = 0;

	pOpL = newOperationNode();

	while(opL->next != NULL)
	{
		opL = opL->next;
	}

	opL->id = idOp;
	opL->quantMachines = nMachines;
	opL->machineAndTime = (int *)malloc(sizeof(int[2][opL->quantMachines]));
	for(i=0; i < opL->quantMachines; i++) 
	{
		opL->machineAndTime[0*opL->quantMachines + i] = arrM[i];
		opL->machineAndTime[1*opL->quantMachines + i] = arrT[i];
	}
	opL->next = pOpL;
}

jobList *checkDataInFile(jobList *jbL, int *idContJob) 
{
	jobList *goThroughList, *auxjbL = NULL;
	FILE *f_JOB = fopen("dados.txt","r");
	int i = 0, cont = 0, arrayM[100], arrayT[100], digitCount = 0, iochar;
	char simpleBuffer[12];//max 10 int digits + 1 negative sign + 1 null char string....if you read more, then you probably don't    have an int there....

	if(f_JOB != NULL)
	 {
		do {
			i = cont = 0;
			while (iochar = getc(f_JOB), iochar != '\n' && iochar != EOF) 
			{
				if(iochar == '#')
				{
					do {
						iochar = getc(f_JOB); 
					}while( iochar != '\n' && iochar != EOF);

					(*idContJob)++;
					auxjbL = newJobNode((*idContJob));
					goThroughList = jbL;
					while (goThroughList->next != NULL) 
					{
						goThroughList = goThroughList->next;
					}
					goThroughList->next = auxjbL;
				}
				if(iochar != '(' && iochar != ')' && iochar != ',' && iochar != ' ' && iochar != '\n') 
				{
					simpleBuffer[digitCount++] = (char) iochar;
				}
				else if(digitCount > 0)
				{
					simpleBuffer[digitCount] = 0; //append null char to end string format
					arrayM[i] = strtol(simpleBuffer, NULL, 0);
					cont++;
					i++;
					digitCount = 0;  //reset buffer to read new number
				}
			}
			i = 0;
			while (iochar = getc(f_JOB), iochar != '\n' && iochar != EOF) 
			{
				if(iochar != '[' && iochar != ']' && iochar != ',' && iochar != ' ' && iochar != '\n') {
					simpleBuffer[digitCount++] = (char) iochar;
				}
				else if(digitCount > 0){
					simpleBuffer[digitCount] = 0; //append null char to end string format
					arrayT[i] = strtol(simpleBuffer, NULL, 0);
					i++;
					digitCount = 0;  //reset buffer to read new number
				}
			}

			if(i > 0){
				goThroughList->nOperations++;
				createOperationFromFile(goThroughList->opL,goThroughList->nOperations,cont,arrayM,arrayT);
			}
			
		}while(iochar != EOF);
	}
	fclose(f_JOB);

	return jbL;
}

void insertJob(jobList *jbL,int *idContJob) 
{
	jobList *auxjbL = NULL, *goThroughList = NULL;
	int opYN;

	system("cls");
	(*idContJob)++;
	auxjbL = newJobNode((*idContJob));
	goThroughList = jbL;
	while (goThroughList->next != NULL) {
		goThroughList = goThroughList->next;
	}
	goThroughList->next = auxjbL;

	printf("Queres adicionar operações ao Job atual? \n");
	while (opYN = yesNo(), opYN == 1) {
		insertOperationNode(goThroughList->opL,&(goThroughList->nOperations));
	}
}

void removeJob(jobList *jbL) 
{
	jobList *y, *atras, *frente, *auxOp;
	int intElemRetirar;

	system("cls");
	y = jbL;
	
	if((y->next)==NULL) { 
		printf("Nao ha operacoes na lista\n"); 
	}
	else {
		printf("Qual o codigo da operacao que deseja remover?\n");
		scanf("%d",&intElemRetirar);

		if(intElemRetirar == jbL->id) {
			system ("cls");
			printf("O elemento foi removido\n");
			listJobNode(jbL->opL,jbL);
			jbL = jbL->next;
			free(y);
		}
		else {
			auxOp = jbL;
			while((intElemRetirar != auxOp->id) && (auxOp->next != NULL)) {
				atras = auxOp;
				auxOp = auxOp->next;
				frente = auxOp->next;
			}
			
			if(intElemRetirar == auxOp->id) {
				atras->next = frente;
				system ("cls");
				printf("O elemento foi removido\n");
				listJobNode(jbL->opL,jbL);
				free(auxOp);
			}
			else {
				system("cls");
				printf("O elemento com o codigo %d nao existe na lista", intElemRetirar);
			}
		}
	}
}

jobList *findJobInList(jobList *node, int k)
 {
	while(node->next != NULL) 
	{
		if (node->id == k) {
			listJobNode(node->opL,node);
			return node;
		} 
		node = node->next;
	}
	return NULL;
}

jobList *findJob(jobList *auxjbL) 
{
	jobList *jobToFind;
	int intElemFind;

	system("cls");
	
	if(auxjbL == NULL) 
	{ 
		printf("Nao ha jobs na lista\n");
		system("cls");
		return NULL;
	}
	else 
	{
		printf("Qual o id do job que deseja encontrar?\n");
		scanf("%d",&intElemFind);

		jobToFind = findJobInList(auxjbL,intElemFind);
		if(jobToFind != NULL)
			return jobToFind;
		
		printf("O elemento com o codigo %d nao existe na lista\n", intElemFind);
		return NULL;
	}
}

void readOperation(operationList *op) 
{
	int i, j, check;

	printf("Quantas maquinas podem ser usadas para esta operacao: ");
	scanf("%d",&op->quantMachines);

	op->machineAndTime = (int *)malloc(sizeof(int[2][op->quantMachines]));
	for (i = 0; i < 2; i++) {
		for (j = 0; j < op->quantMachines; j++) {
			if(i == 0) {
				do {
					if( j > 0) {
						check = 0;
						printf("Qual o id da maquina que deseja usar: ");
						scanf("%d",&op->machineAndTime[i*op->quantMachines + j]);

						check = searchEqualMachine(op,op->machineAndTime[i*op->quantMachines + j],j);
						if(check == 0) {
							printf("\nMaquina %d ja esta a ser utilizada!!! Escolha Outra\n\n", op->machineAndTime[i*op->quantMachines + j]);
						}
					}
					else 
					{
						check = 1;
						printf("Qual o id da maquina que deseja usar: ");
						scanf("%d",&op->machineAndTime[i*op->quantMachines + j]);
					}
				}while(check == 0);
			}
			else 
			{
				printf("Quanto tempo a maquina %d vai demorar: ", op->machineAndTime[0*op->quantMachines + j]);
				scanf("%d",&op->machineAndTime[i*op->quantMachines + j]);
			}
		}
	}
}

void insertOperationNode(operationList *op, int *nOperations) 
{
	operationList *auxOp;
	int id = 1;

	auxOp = newOperationNode();
	system("cls");

	if(auxOp==NULL) 
	{ 
		printf("Nao ha espaco na memoria. Impossivel inserir!!!\n\n");
		system("pause");
	}
	else 
	{
		while((op->next) != NULL) {
			id = op->id + 1;
			op = op->next;
		}

		(*nOperations)++;
		op->id = id;
		readOperation(op);
		op->next = auxOp;
	}
}

void removeOperation(operationList *opL, int *nOperations) 
{
	operationList *y, *atras, *frente, *auxOp;
	int intElemRetirar;

	system("cls");
	y = opL;
	
	if((y->next)==NULL) { 
		printf("Nao ha operacoes na lista\n"); 
	}
	else 
	{
		printf("Qual o codigo da operacao que deseja remover?\n");
		scanf("%d",&intElemRetirar);

		if(intElemRetirar == opL->id) 
		{
			system ("cls");
			
			(*nOperations)--;
			printf("O elemento foi removido\n");
			listOperationNode(opL);
			opL = opL->next;
			free(y);
		}
		else 
		{
			auxOp = opL;
			while((intElemRetirar != auxOp->id) && (auxOp->next != NULL)) 
			{
				atras = auxOp;
				auxOp = auxOp->next;
				frente = auxOp->next;
			}
			
			if(intElemRetirar == auxOp->id) 
			{
				atras->next = frente;
				system ("cls");
				
				(*nOperations)--;
				printf("O elemento foi removido\n");
				listOperationNode(auxOp);
				free(auxOp);
			}
			else 
			{
				system("cls");
				printf("O elemento com o codigo %d nao existe na lista", intElemRetirar);
			}
		}
	}
}

void editOperation(operationList *opL) 
{
	int intElemEditar;

	system("cls");
	
	if((opL->next) == NULL) 
	{ 
		printf("Nao ha operacoes na lista\n"); 
	}
	else 
	{
		printf("Qual o codigo da operacao que deseja editar?\n");
		scanf("%d",&intElemEditar);

		if(intElemEditar == opL->id) {
			system ("cls");
			printf("O elemento a ser editado\n");
			listOperationNode(opL);
			readOperation(opL);
		}
		else 
		{
			while((intElemEditar != opL->id) && (opL->next != NULL)) 
			{
				opL = opL->next;
			}
			
			if(intElemEditar == opL->id) {
				system ("cls");
				printf("O elemento a ser editado\n");
				listOperationNode(opL);
				readOperation(opL);
			}
			else 
			{
				system("cls");
				printf("O elemento com o codigo %d nao existe na lista", intElemEditar);
			}
		}
	}
}

void writeInFile(operationList *opL, int id, FILE *f_JOB) 
{
	int i;

	fprintf(f_JOB,"#%d\n", id);
	while(opL->next != NULL) {
		if(opL->quantMachines == 1) 
		{
			fprintf(f_JOB,"(%d)\n", opL->machineAndTime[1*opL->quantMachines + 0]);
			fprintf(f_JOB,"[%d]\n", opL->machineAndTime[0*opL->quantMachines + 0]);
		}
		else 
		{
			for(i = 0; i < opL->quantMachines; ++i) 
			{
				if( i == 0)
					fprintf(f_JOB,"(%d,", opL->machineAndTime[1*opL->quantMachines + i]);
				else if((opL->quantMachines - i) != 1)
					fprintf(f_JOB,"%d,", opL->machineAndTime[1*opL->quantMachines + i]);
				else
					fprintf(f_JOB,"%d)\n", opL->machineAndTime[1*opL->quantMachines + i]);
			}
			for(i = 0; i < opL->quantMachines; ++i) 
			{
				if( i == 0)
					fprintf(f_JOB,"[%d,", opL->machineAndTime[0*opL->quantMachines + i]);
				else if((opL->quantMachines - i) != 1)
					fprintf(f_JOB,"%d,", opL->machineAndTime[0*opL->quantMachines + i]);
				else
					fprintf(f_JOB,"%d]\n", opL->machineAndTime[0*opL->quantMachines + i]);
			}
		}
		opL = opL->next;
	}
}

void saveDataInFile(jobList *jbL, char *fileName) 
{
	FILE *f_JOB = fopen(fileName,"w");

	if(f_JOB != NULL) 
	{
		while(jbL->next != NULL) {
			writeInFile(jbL->opL,jbL->id,f_JOB);
			jbL = jbL->next;
		}
	}
	fclose(f_JOB);
}

void deallocate(jobList *node)
{	
	while(node->next != NULL) {
		while (node->opL->next != NULL) 
		{
			free(node->opL->machineAndTime);
			node->opL->machineAndTime = NULL;
			free(node->opL);
			node->opL = NULL;
			node->opL = node->opL->next;
		}
		free(node);
		node = node->next;
	}
}

int yesNo() 
{
	char buf[1024];

	fflush(stdin);

	while(TRUE) 
	{
		printf("sim ou nao: ");

		if(!fgets(buf, sizeof(buf), stdin)) 
		{
			printf("Algo deu errado tente de novo\n\n");
			printf("Error: %d\n", errno);
			system("pause");
		}
		errno = 0;
		buf[strcspn(buf, "\n")] = 0;
		strlwr(buf);

		if(errno == EINVAL) {
			printf("Erro na conversao: %d""\n", errno);
			exit(0);
		}
		if(errno == ERANGE) {
			printf("A opcao selecionada e demasiado grande ou demasiado pequena.""\n\n");
		}
		else if((strcmp(buf,"sim") != 0) && (strcmp(buf,"nao") != 0)) {
			printf("Responda sim ou nao.\n\n");
		}
		else if(strcmp(buf,"sim") == 0) {
			return 1;
		}
		else if(strcmp(buf,"nao") == 0) {
			return 0;
		}
	}
}

void menu(int *option) 
{
	int success;
	char *endptr, buf[1024];
	
	fflush(stdin);
	
	do 
	{
		system("cls");
		success = 0;

		_printf_p("\t\t\tMENU\n"
				"\t0 - Sair\n\n"
				"\t1 - Listar Job/s\n"
				"\t2 - Inserir Job\n"  
				"\t3 - Remover Job\n\n"
				"\t4 - Inserir Operacao no Job\n"       
				"\t5 - Remover Operacao no Job\n"       
				"\t6 - Editar Operacao no Job\n"
				"\tOpcao: ");

		if(!fgets(buf, sizeof(buf), stdin)) 
		{
			printf("Algo deu errado tente novamente\n\n");
			printf("Error: %d\n", errno);
			system("pause");
		}
		errno = 0; // reset error number
		(*option) = strtol(buf, &endptr, 10);
		if(errno == EINVAL) 
		{
			printf("Erro na conversao: %d\n", errno);
			exit(0);
		}
		if(errno == ERANGE) 
		{
			printf("A opcao selecionada e demasiado grande ou demasiado pequena.\n\n");
			system("pause");
		}
		else if (((*option) > 6) || ((*option) < 0)) 
		{
			printf("Coloque um numero entre 0 a 7.\n\n");
			system("pause");
		}
		else if (endptr == buf)	
		{
			// no character was read.
			printf("Nenhum caracter lido. Coloque um numero entre 0 a 7.\n\n");
			system("pause");
		}
		else if ((*endptr) && (*endptr != '\n')) 
		{
			// *endptr is neither end of string nor newline, so we didn't convert the *whole* input.
			printf("Nenhum caracter lido. Coloque um numero entre 0 a 7.\n\n");
			system("pause");
		}
		else 
		{
			success = 1;
		}
	}while(!success);
}