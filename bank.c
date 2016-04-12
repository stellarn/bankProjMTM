/* bank project
 Yahel Tsufim 304952898
 Noam Stolero 201581683
 file:bank.c
 */

#include "bank.h"

void initBank(bank*);

#define CLIENTSHEAD(STRUCT) STRUCT->clientsList.head
#define CLIENTSTAIL(STRUCT) STRUCT->clientsList.tail
/*********_Bank_Creation_Functions_START_******************/


/* The bank instance, will be used only in this file*/
bank* masterBank=NULL;


/*Initialize bank struct.*/
void initBank(bank* masterBank){
    masterBank->balance=0;
    masterBank->numOfActiveLoans=0;
    masterBank->numOfBranch=0;
    masterBank->numOfClients=0;
    masterBank->yearProfit=0;
}


/*create a single bank struct.*/
void createBank(){
    masterBank=ALLOC(bank, 1);
    initBank(masterBank);
    createBankClientList();
    getName(&(masterBank->name), BANKNAMEMAX, "please enter bank name:\n");
}


/*create and init client list*/
void createBankClientList()
{
    client* tempHead=NULL , *tempTail=NULL;

    tempTail=ALLOC(client,1);
    tempTail->next=NULL;

    tempHead=ALLOC(client,1);
    tempHead->next=tempTail;


    CLIENTSHEAD(masterBank)->next=tempHead;

    return;
}

/*********_Bank_Creation_Functions_END_******************/



/*********_Update_bank_Fields_START_******************/


/*update bank balance.*/
void updateBankBalance(amount money,addremove remove){
    if (remove) {
        if (masterBank->balance<=0){
            printf("no balance");
            return;
        }
        masterBank->balance-=money;
    }
    masterBank->balance+=money;
}

/*update number of active loans.*/
void updateNumOfActiveLoans(addremove remove){
    if (remove) {
        if (masterBank->numOfActiveLoans<=0) {
            printf("no active loans");
            return;
        }
        masterBank->numOfActiveLoans--;
    }
    masterBank->numOfActiveLoans++;
}



/*update number of branches.*/
void updateNumOfBranches(addremove remove){
    if (remove){
        if (masterBank->numOfBranch<=0) {
            printf("no branches\n");
            return;
        }
        masterBank->numOfBranch--;
    }
    masterBank->numOfBranch++;
}

/*update number of clients of the bank.*/
void updateNumOfBankClients(addremove REMOVE){
    masterBank->numOfClients++;
}


/*delete a client from the bank.*/
try deleteBankClient(accountNum acc){
    client *getPreClient=NULL, *getNextClient=NULL,
    		*clientToBeDeleted=NULL;

    /*find bank client;*/
    getPreClient=getBankClient(acc, &getNextClient);

    if (getPreClient==NULL) {
        return CLIENTNOTFOUND;
    }

    clientToBeDeleted=getPreClient->next;

    /*update bank balance / client size*/
    updateBankBalance(clientToBeDeleted->balance, REMOVE);
    updateNumOfBankClients(REMOVE);
    
    /*delete the Client*/
    getPreClient->next=getNextClient;
    
    FREE(clientToBeDeleted);
    return SUCCESS;
}

/*add a new client to the bank.*/
void addNewClientToBank(client* newClient){
    client *tempNext=NULL;

    tempNext = CLIENTSHEAD(masterBank)->next;

    CLIENTSHEAD(masterBank)->next=newClient;
    newClient->next=tempNext;

    masterBank->numOfClients++;
}

/*********_Update_bank_Fields_END_******************/


/*return number clients of bank.*/
int BankNumberOfClients(){
    return masterBank->numOfClients;
}



/*find a client in bank Client list.*/
client* getBankClient(accountNum acc, client** nextClient){
    client* preClient=CLIENTSHEAD(masterBank);
    while (preClient->next!=CLIENTStAIL(masterBank))
    {
    	if (preClient->next->accNum==acc){

    		if (nextClient==GETSPECIFIC){
    			return preClient->next;
    		}
    		*nextClient=preClient->next;
    		return preClient;
    	}
    }
    return NULL;
}

/*********_Information_Functions_START_******************/

/*return current number of branches.*/
int getNumOfBranches(){
    return masterBank->numOfBranch;
}


/*check if bank is full.*/
int isBankFull(){
    if (masterBank->numOfClients<=MAXBANKCLIENTS) {
        return FALSE;
    }
    return TRUE;
}


int clientNumberOfBank_REC(client *head, int *biggestBalance){

	if (head==CLIENTSHEAD(masterBank) && head->next==CLIENTSTAIL(masterBank)){
		*biggestBalance=0;
		return 0;
	}
	if (head->next==CLIENTSTAIL(masterBank)){

	}

}


/* check which are the client/s with biggest balance and return their number and
 * biggest balance.
 */
int clientNumberOfBank(int *biggestBalance){
	client* current=CLIENTSHEAD(masterBank);
	int counter=0;
	*biggestBalance=0;

	while (current->next!=CLIENTSTAIL(masterBank)){
		if(*biggestBalance<=current->balance){
			counter++;
			*biggestBalance=current->balance;
		}
		current=current->next;
	}
	return counter;

}

/*********_Information_Functions_END_******************/


/* Delete the bank*/
void deleteBank(){
    deleteAllBranches();
    FREE(CLIENTSHEAD(masterBank));
    FREE(CLIENTSTAIL(masterBank));
    FREE(masterBank);
}
