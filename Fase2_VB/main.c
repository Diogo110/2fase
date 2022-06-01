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
#include "functions.c"


int main() 
{
	jobList *jbL = NULL, *jobToFind = NULL;
	int option, idContJob = 1;
	
	system("cls");
    jbL = newJobNode(idContJob);
    checkDataInFile(jbL,&idContJob);

	do {
		menu(&option);

		switch(option) {
			default:
			case 0: printf("Ate a proxima!!\n\n");
					break;
			case 1: system("cls");
					ListJobInOrder(jbL);
					system("pause");
					break;
			case 2: insertJob(jbL,&idContJob);
					break;
			case 3: removeJob(jbL);
					system("pause");
			        break;
            case 4: jobToFind = findJob(jbL);
                    insertOperationNode(jobToFind->opL,&(jobToFind->nOperations)); 
                    system("pause");
                    break;
            case 5: jobToFind = findJob(jbL);
                    removeOperation(jobToFind->opL,&(jobToFind->nOperations));
                    system("pause");
                    break;
            case 6: jobToFind = findJob(jbL);
                    editOperation(jobToFind->opL);
                    system("pause");
                    break;
		}
	}while(option != 0);
	
	saveDataInFile(jbL,"dados.txt");

	deallocate(jbL);
	
	return 0;
}