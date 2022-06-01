#ifndef _HEADER
#define _HEADER

typedef struct operationList {
	int id;
	int quantMachines;
	int *machineAndTime;
	struct operationList *next;
}operationList;

typedef struct jobList {
	int id;
	int nOperations;
	int height;
	operationList *opL;
	struct jobList *next;
}jobList;



int yesNo();
void menu(int *option);

jobList *newJobNode(int key);
operationList *newOperationNode();


void listOperationNode(operationList *op);
void listJobNode(operationList *auxOpL, jobList *jb);
void ListJobInOrder(jobList *n);

int searchEqualMachine(operationList *m, int elemToFind, int currentPosition);

void createOperationFromFile(operationList *opL, int idOp, int cont, int arrM[], int arrT[]);
jobList *checkDataInFile(jobList *jbL, int *idContJob);

void insertJob(jobList *jbL,int *idContJob) ;

void removeJob(jobList *jbL);

jobList *findJobInList(jobList *root, int key) ;
jobList *findJob(jobList *auxjbL);

void readOperation(operationList *op);
void insertOperationNode(operationList *op, int *nOperations);
void removeOperation(operationList *opL, int *nOperations);
void editOperation(operationList *opL);

void writeInFile(operationList *opL, int id, FILE *f_JOB);
void saveDataInFile(jobList *jbL, char *fileName);

void deallocate(jobList *node);

#endif