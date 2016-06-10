//
//  genericTree.c
//  bankMTM
//
//  Created by Noam Stolero on 6.6.2016.
//  Copyright © 2016 Noam Stolero. All rights reserved.
//


#include "common.h"

/* macro for swapping data */
#define SWAP(D1,D2,TEMP) TEMP=D1->data; \
                    D1->data=D2->data; \
                    D2->data=TEMP \




/*create tree node*/
genTree * create_Tree(){
    
    genTree *temp;
    
    temp=ALLOC(genTree, 1);
    temp->data=NULL;
    temp->left=NULL;
    temp->right=NULL;
    return temp;
}


/*add node to tree*/
genTree * add_new_node(genTree* t,void* data,genCmp cmp){
    comparison c;
    if(!t){
        t=create_Tree();
        t->data=data;
        return t;
    }
    
    c=cmp(t->data,data); /* call designated compare function */
    if (c==GREATER){
        t->left=add_new_node(t->left, data, cmp);
    }else{
        t->right=add_new_node(t->right, data, cmp);
    }
    
    return t;
}


/* find biggest node in given tree */
genTree * findBiggestNode(genTree* t){
  
    while (t!=NULL && t->right!=NULL){
        t=t->right;
    }
    return t;
}

/* find smallestNode in given tree */
genTree * findSmallestNode(genTree* t){
    
    while (t!=NULL && t->right!=NULL){
        t=t->left;
    }
    return t;
}


/* delete a leaf from tree */
genTree * deleteLeaf(genTree *t, genDelete gDelete){
    gDelete(t->data);
    FREE(t);
    return NULL;
}

/* remove a node from tree */
genTree * remove_node(genTree* t, void* data, genDelete gDelete, genCmp cmp){
    genTree* toSwap, *toDelete, *parent=NULL;
    void *temp;
    
    if (!t){
        return NULL;
    }
    
    toDelete=find_Node_Parent(t, data,&parent, cmp);    /*find node and its parent */
    
    if (toDelete==NULL) /* no node found */
        return NULL;
    
    if(toDelete->left==NULL && toDelete->right!=NULL){
        toSwap=findSmallestNode(toDelete->right); /*find smallest node in right tree */
        SWAP(toDelete, toSwap, temp); /* swap smallest node with the one to delete*/
        /* call this function again on right tree */
        toDelete->right=remove_node(toDelete->right,data, gDelete, cmp);
        return toDelete;
    }
    else if (toDelete->left!=NULL){
        toSwap=findBiggestNode(toDelete->left); /*find biggest node in left tree */
        SWAP(toDelete, toSwap, temp); /* swap biggest node with the one to delete*/
        /* call this function again on right tree */
        toDelete->left=remove_node(toDelete->left, data,gDelete, cmp);
        return toDelete;
    }
    
    /* leaf case */
    if (parent){ /*if there is a parent */
        if (parent->left==toDelete)
            parent->left=NULL;
        else
            parent->right=NULL;
        deleteLeaf(toDelete, gDelete);
        return t;
    }
    /* root case */
    deleteLeaf(toDelete, gDelete);
    return NULL;
}





/*find a node and his parent*/
genTree * find_Node_Parent(genTree * t ,void* data ,genTree ** parent,genCmp cmp){
    comparison c;
    
    
    if (!t){
        if (parent)
            *parent=NULL;
        return NULL;
    }
    
    c=cmp(t->data,data);
    
    if (c==EQUAL){
        return t;
    }
    if (parent)
        *parent=t;
    if (c==SMALLER)
        return find_Node_Parent(t->right, data, parent, cmp);
    else
        return find_Node_Parent(t->left, data, parent, cmp);
}


void print_List(genLinked* t, genPrint print)
{
	if(!t)
		return;
    print(t->data);
    print_List(t->next, print);
}

double average_key(genTree* t,int* amount, genValue val){
	int leftAm=0,rightAm=0;
	double leftAv=0,rightAv=0,myAv;
	if(!t){
		*amount = 0;
		return 0;
	}
	leftAv = average_key(t->left,&leftAm,val);
	rightAv = average_key(t->right,&rightAm,val);
	*amount = leftAm + rightAm + 1;
	myAv = val(t->data);
	myAv += (leftAv * leftAm) + (rightAv * rightAm);
	return (myAv/ *amount);
}




/*delete all tree*/
void free_list(genTree** t,genDelete gdelete)
{
    if(!*t)
        return;
    free_list(&(*t)->left,gdelete);
    free_list(&(*t)->right,gdelete);
    gdelete((*t)->data);
    FREE(*t);
    *t=NULL;
    
}


void free_linked_list(genLinked** t,genDelete gDelete)
{
    if(!*t)
        return;
    free_linked_list(&(*t)->next, gDelete);
    FREE(*t);
    *t=NULL;
    
}





/* create linked list from a certain data node */
genLinked * find_node(genTree* t, void* data, genCmp cmp)
{
    genLinked *left, *right;
    if(!t)
        return NULL;
    left=find_node(t->left, data, cmp);
    right=find_node(t->right, data, cmp);
    
    if(cmp(t->data,data)==EQUAL){
        genLinked *me;
        me = ALLOC(genLinked, 1);
        me->data = t->data;
        me->next=NULL;
        right = merge_LinkedList(me,right);
    }
    
    return merge_LinkedList(left,right);
    
}


/* merge two linked list */
genLinked* merge_LinkedList(genLinked* a,genLinked* b)
{
    genLinked* new=a;
    if(a==NULL)
        return b;
    if(b==NULL)
        return a;
    while(a->next!=NULL){
        a=a->next;
    }
    a->next=b;
    return new;
}



/*sort linked list */

genLinked* sortLinkedList(genLinked* l, genCmp cmp){
    comparison c;
    void * temp;
    genLinked *next;
    if (!l)
        return NULL;
    
    next=sortLinkedList(l->next, cmp);
    if (next){
        c=cmp(l->data,next->data);
        if (c==GREATER){
            SWAP(l,next, temp);
        }
        l->next=sortLinkedList(l->next, cmp);
    }
    
    
    return l;
}







