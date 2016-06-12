/* bank project
 Yahel Tsufim 304952898
 Noam Stolero 201581683
 file:branch.c
 */

/* ---------i chose right to be the smaller and left to be the bigger-------- */
/*-----------------------INCLUDE------*/
#include "bank.h"

/*--------------------DEFINE---------------*/
#define MAXBRANCHCLIENT 1000
#define N 500
#define HOURSINDAY 24
#define MILLION 1e6
#define MINBALANCE 1
#define MAXBALANCE 500

/*---------------------LOCAL BRANCHLIST-----------------*/
static genTree* branchRoot;
/*--------------------LOCAL FUNCTION DECLERATION--------*/
void initBranch(branch*);/*init branch struct*/
branch *createBranch();/* create a new branch. get information from user */
void updateCurrentClient(branchID ,addremove);/*update amount of clients in branch*/
void deleteBranchFields(branch*);/* delete all the fields of a certain branch */
boolean addClientConditions(); /* check if possible to add new client to the bank */
branch* findBranch(genTree* , branchID );
branchID getBranchID(availble checkif);/* get branch ID from user, including check if the id is already in use*/
int getTime(char*); /*get hours from user.*/
int isBranchFull(branch *);/*check if branch is full (has more room from clients)*/

/*        TEMPORARY      */
int countClients(genTree* , amount* ,genCmp);
int compareClientsWithBiggerBalance(client* client, amount *balance);
int compareClientsWithBiggerLoans(client* client, amount *balance);
void printClientDetails(client* client);
double getNumOfClientsInBranch(branch* b);
/*        TREE       */


/*----------------------------------------------CODE BEGIN'S HERE--------------------------------------------*/




/**************** NOAM ADDED ************************/

/* find a clients in a certain branch by ID or balance */
void findClientInGivenBranch (){
    branch *b;
    b=getBranch(getBranchID(EXIST));
    if (b==NULL)
        printf("Action canceled\n");
    else
        findClient(b->clientList);
}



/***************** BRANCH FUNCTIONS FOR GENERAL PURPUSES *****************/

comparison compare_Branch(branch* a,branch *b)
{
	if(a->brID > b->brID)
		return GREATER;
	if(a->brID < b->brID)
		return SMALLER;
	return EQUAL;
}


double getNumOfClientsInBranch(branch* b)
{
	return b->currentClients;
}



void deleteBranchFields(branch* to_be_deleted)
{
    FREE(to_be_deleted->branchName);
    /*delete!!!!   clearClientTree(to_be_deleted->clientList);*/
    free_list(&to_be_deleted->clientList, (genDelete)&freeClient);
    FREE(to_be_deleted);
}


comparison compareClientsWithBiggerBalance(client* client, amount *balance){
	if(client->balance > *balance)
		return GREATER;
	if(client->balance < *balance)
		return SMALLER;
	return EQUAL;
}

comparison compareClientsWithBiggerLoans(client* client, amount *balance){
	if(client->debt > client->balance)
		return GREATER;
	if(client->debt > client->balance)
		return SMALLER;
	return EQUAL;
}

void printClientDetails(client* client)
{
	printf("Client account number : %d\n",client->accNum);
	printf("Client Balance : %f\n",client->balance);
}



/***************** BRANCH FUNCTIONS FOR GENERAL PURPUSES *****************/

void createBranchList()
{
	branchRoot = NULL;;
}



try addNewBranch()
{
    /*put the new branch in the end of the branch list*/
    branch* newBranch;

    if(getNumOfBranches() == N){
        printf("bank has maximum number of branches.\n");
        return MAX_BANK_REACHED;
    }
    newBranch = createBranch();
    branchRoot = add_new_node(branchRoot,newBranch,(genCmp)(&compare_Branch));
    updateNumOfBranches(ADD);/*update branch list on addition of bank*/
    printf("Add new branch finished successfully\n");
    return SUCCESS;
}


genTree* createBranchClientList()
{
	return NULL;
}

try addNewClientToBranch()
{
    /*prepare data to receive client*/
    branch *temp;
    client *newClient;
    boolean check;
    branchID brID;
    
    printf("Starting new client registry:\n");
    check=addClientConditions();
    if(check==FALSE)
    	return FAIL;
#ifdef TEST
    brID=1;  /* make branch id 1 for test purposes */
#else
    brID = getBranchID(EXIST);
#endif
    if(brID == CANCEL){
        printf("Action aborted\n");
        return CANCEL;
    }
    
    temp = getBranch(brID);
    
    if (isBranchFull(temp)) {
        printf("the branch is full\n");
        return FAIL;
    }
    newClient = getDetailsFromUser(temp->brID,temp->bankName);
    

    /*inform branch and bank on new client*/
    temp->currentClients++;
    addNewClientToBank(newClient);
    temp->clientList = add_new_node(temp->clientList,newClient,(genCmp)(&cmpClient));
    printf("Add new client finished successfully\n");
    return SUCCESS;
}

boolean addClientConditions(){
if(getNumOfBranches()==0){
           printf("first add a branch\n");
           return FALSE;
   }
   if (isBankFull()) {
       printf("The bank is full\n");
       return FALSE;
   }
   return TRUE;
}


#ifdef BANK_AHAMELIM
void clientNumberWithGivenBalance()
{
	int numberOfClients;
    amount balance;
    branch *tempBranch;
    branchID brID;
    if(getNumOfBranches()==0){
        printf("no branches\n");
        return ;
    }
    brID = getBranchID(EXIST);
    if(brID == CANCEL){
        printf("Aborted the action\n");
        return;
    }
    
    tempBranch = getBranch(brID);
    getDouble(&balance, "please enter balance:\n");
    numberOfClients = countClients(tempBranch->clientList,&balance,(genCmp)&compareClientsWithBiggerBalance);
    printf("the number of clients with : %f\n  in balance is :%d\n",balance,numberOfClients);
}
#endif

void printClientAccountsNumberAndBalance()
{
	branch *tempBranch;
    branchID brID;
    if(getNumOfBranches()==0){
        printf("no branches\n");
        return ;
    }
    brID = getBranchID(EXIST);
    if(brID == CANCEL){
        printf("Aborted the action\n");
        return;
    }
    tempBranch = getBranch(brID);
    print_tree(tempBranch->clientList,(genPrint)&printClientDetails);

}
void clientNumberWithBiggerLoansThanBalance()
{
	int numberOfClients;
    branch *tempBranch;
    branchID brID;
    if(getNumOfBranches()==0){
        printf("no branches\n");
        return ;
    }
    brID = getBranchID(EXIST);
    if(brID == CANCEL){
        printf("Aborted the action\n");
        return;
    }
    
    tempBranch = getBranch(brID);
    numberOfClients = countClients(tempBranch->clientList,NOCHECK,(genCmp)&compareClientsWithBiggerLoans);
    printf("amount of clients with bigger loans then balance : %d",numberOfClients);
}


void averageNumberOfAccountsInBranches()
{
	int numOfBranches=0;
	double clientsAverage=0;
	numOfBranches = average_key(branchRoot,&numOfBranches,(genValue)&getNumOfClientsInBranch);
	printf("number of branches : %d\n"
			"Average number of clients in single branch : %f\n",
			numOfBranches,clientsAverage);
}


try deleteAllBranchClients(branchID id)
{
    branch *temp=NULL;
    if (!id) {/*if no ID has been received. get branch id from user*/
        getInt(&id, "please enter branch ID:\n");
    }
    temp = getBranch(id);
    /*delete all clients*/
    temp->balance = 1;
    temp->currentClients = 0;
    return SUCCESS;
}

try deleteBranchClient(branchID brID,accountNum acc)
{
    /*set variables*/
    client* tempClient=NULL;
    branch* tempBranch;

    tempBranch=getBranch(brID);
    if ((tempClient = getClient(tempBranch->clientList,acc,NULL)) == NULL ) {
           printf("client not in branch.\n");/*if the  client is not in branch*/
           return CLIENTNOTFOUND;
       }
    /*update branch on leaving costumer*/
    updateBranchBalance(tempClient->brID,tempClient->balance, REMOVE);
    if(tempClient->debt>0)
    		updateBranchLoan(brID,REMOVE);
    tempBranch->clientList = remove_node(tempBranch->clientList,tempClient,(genDelete)&freeClient,(genCmp)(&cmpClient));
    tempBranch->currentClients--;
    return SUCCESS;
}


try  deleteBranch(branchID brID)
{
	branch* tempBranch;
    if(getNumOfBranches()==0){
    	printf("There are no active branches\n");
    	return SUCCESS;
    }

    if(brID==NOCHECK){
        brID=getBranchID(EXIST);/*receive branch from user*/
        if(brID == CANCEL){
            printf("Aborted the action\n");
            return CANCEL;
        }
    }
    tempBranch = getBranch(brID);
    branchRoot = remove_node(branchRoot,(void*)tempBranch,(genDelete)(&deleteBranchFields),(genCmp)(&compare_Branch));
    updateNumOfBranches(REMOVE);  /* decrease amount of branches in bank*/
    return SUCCESS;
}

void deleteAllBranches()
{

	/*delete!!!! clearBranchTree(branchRoot);*/
    free_list(&branchRoot, (genDelete)&deleteBranchFields);
}

/*----------------------------------------------------*/

try updateBranchBalance(branchID brID, amount am,addremove remove)
{
    branch *tempBranch;
    ;
    if ((tempBranch = getBranch(brID))==NULL){/*if branch not found*/
        printf("branch not found.\n");
        return BRANCHNOTFOUND;
    }
    if (remove==REMOVE) {/*if needed to decrease (client leave, has less money, etc.)*/
    	if(tempBranch->balance - (am/MILLION) < MINBALANCE){
    		printf("branch balance is at minimum, can't withdraw more money\n");
    		return FAIL;
    	}
    	tempBranch->balance = tempBranch->balance - (double)am/MILLION;
        return SUCCESS;
    }
    else{
    	if(tempBranch->balance - (am/MILLION) > MAXBALANCE){
    		printf("branch balance is at maximum, can't deposit more money\n");
    	    return FAIL;
    	}
    	tempBranch->balance = (double)am/MILLION + tempBranch->balance;
    }
    return SUCCESS;
}

void updateBranchLoan(branchID brID,addremove remove)
{
    branch *tempBranch;
    tempBranch = getBranch(brID);
    if(remove)
    	tempBranch->numOfActiveLoans--;
    else
    	tempBranch->numOfActiveLoans++;
}

void updateCurrentClient(branchID brID,addremove remove){
    branch *tempBranch;
    tempBranch = getBranch(brID);
    if(remove)
    	tempBranch->currentClients--;
    else
    	tempBranch->currentClients++;
}


int isBranchFull(branch *tempBranch)
{
    if (tempBranch->currentClients<=MAXBRANCHCLIENT) {
        return FALSE;
    }
    return TRUE;
}


comparison comp_Branch_to_ID(branch* a, branchID* b){
	if(a->brID > *b)
		return GREATER;
	if(a->brID < *b)
		return SMALLER;
	return EQUAL;
}

branch* getBranch(branchID brID)
{
    return findBranch(branchRoot,brID);
}



client* getBranchClient(accountNum acc, branchID brID)
{
    branch *tempBranch=NULL;
    tempBranch=getBranch(brID);
    return getClient(tempBranch->clientList,acc,NULL);
}



/*---------------HELP SEARCH IN TREE BRANCH-------------------*/
boolean checkBranchID(branchID brID)
{
    if(findBranch(branchRoot,brID) != NULL)
    	return TRUE;
    return FALSE;
}



branch* findBranch(genTree* root, branchID brID){
    genTree* b;
    b=find_Node_Parent(branchRoot,&brID,NOCHECK,(genCmp)&comp_Branch_to_ID);
    if (b) {
        return (branch*)b->data;
    }
    return NULL;

}


/*------------------------------------RECIEVE DATA FROM USER-------------------------------*/

int getTime(char *output){
    int time;
    do{
        getInt(&time, output);
    }while(time<0 || time>HOURSINDAY);
    return time;
}


branchID getBranchID(availble checkif){
    branchID brID;
    boolean flag = FALSE;
    do{
        getInt(&brID,"please enter branch id (1-500): (type 0 to cancel)\n");
        if (brID==CANCEL){
            return CANCEL;
        }
        if (brID>N || brID<0) {
            printf("Branch id not in range (1-500)\n");
            continue;
        }
        if(checkBranchID(brID)){
            if(checkif==EXIST){
                flag = TRUE;
            }else{
                printf("The branch id is taken. enter a new ID.\n");
            }
        }else if (checkif==NOTEXIST){
            flag=TRUE;
        }else{
            printf("Branch not found. enter new ID\n");
        }
    }while(flag == FALSE); /* check if id doesn't exist*/
    
    return brID;
    
}


/*-----------------------------INFORMATION FUNCTIONS-------------------*/

void printBranchInfo()
{
	branchID brID;
	branch *tempBranch;
    brID = getBranchID(EXIST);
    if(brID == CANCEL)
    {
        printf("Aborted the action\n");
        return;
    }
	tempBranch = getBranch(brID);
	printf("Branch name: %s\n",tempBranch->branchName);
	printf("Branch Bank name: %s\n",tempBranch->bankName);
	printf("Branch ID : %d\n",tempBranch->brID);
	printf("Number of clients: %d\n",tempBranch->currentClients);
	printf("Number of loans: %d\n",tempBranch->numOfActiveLoans);
	printf("Opening time: %d\n",tempBranch->openTime);
	printf("Closing time: %d\n",tempBranch->closeTime);
	printf("Branch balance: %g\n",tempBranch->balance);
	printf("Yearly profit: %g\n",tempBranch->yearProfit);
}

/*----------ADD NEW BRANCH FUNCTIONS-------------*/
branch *createBranch()/* create branch, receive data from user */
{
	branch *newBranch;
	printf("Add new branch start:\n");
    newBranch = ALLOC(branch,1);
	initBranch(newBranch);

#ifdef TEST
	newBranch->bankName = getBankName();
    newBranch->branchName=str_dup(testName);
    testName[0]++;
    newBranch->brID=testBID++;
    newBranch->openTime=1;
    newBranch->closeTime=1;
    newBranch->clientList=createBranchClientList();
#else

    /*receive data from user*/
	getName(&newBranch->branchName,MAXNAME,"please enter branch name:\n");
    newBranch->bankName = getBankName();
    newBranch->brID= getBranchID(NOTEXIST);
    newBranch->openTime = getTime("please enter opening time (between 0-23)\n");
    newBranch->closeTime =getTime("please enter closing time (between 0-23)\n");
    newBranch->clientList=createBranchClientList();    /*create the client list of the branch*/
    
#endif
    

    return newBranch;
}


void initBranch(branch *brancInit)
{
    brancInit->brID = 0;
    brancInit->balance=0;
    brancInit->openTime=0;
    brancInit->closeTime=0;
    brancInit->currentClients=0;
    brancInit->numOfActiveLoans=0;
    brancInit->balance = 1.0;
    brancInit->yearProfit = 1.0;

}

/*---------------SEARCH CLIENT FUNCTIONS-----------*/
int countClients(genTree* root, amount* balance,genCmp cmp){
	int numberOfClients;
	comparison c;
	if(root == NULL)
		return 0;
	numberOfClients = countClients(root->right,balance,cmp);
	numberOfClients +=countClients(root->left,balance,cmp);
	c = cmp(root->data,balance);
	if(balance){
		if(c==EQUAL)
			numberOfClients += 1;
	}
	else{
		if(c==GREATER)
			numberOfClients += 1;
	}
	return numberOfClients;
}



/*DELETE FROM TREE*/



