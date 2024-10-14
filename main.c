#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "leak_detector_c.h"

#define MAXLEN 25

typedef struct customer {
  char name[MAXLEN+1];
  int points;
}customer;

typedef struct bstnode {
  customer* custPtr;
  int size;
  struct bstnode* left;
  struct bstnode* right;
}bstnode;

void freeTree(bstnode *current_ptr);
void swap(customer** a, customer** b);
int comparePoints(customer** a, customer** b);
int partition(customer** a, int left, int right);
void quickSort(customer** arr, int low, int high);
bstnode* insert(bstnode *root, bstnode *element);
bstnode* create_node(char* name, int point);
bstnode* findNode(bstnode *current_ptr, char* name);
bstnode* parent(bstnode *root, bstnode *node);
bstnode* minVal(bstnode *root);
bstnode* maxVal(bstnode *root);
int isLeaf(bstnode *node);
int hasOnlyLeftChild(bstnode *node);
int hasOnlyRightChild(bstnode *node);
bstnode* delete(bstnode* root, char* name, bstnode** end);
void updateSize(bstnode *current_ptr, bstnode *node);
int findHeight(bstnode *current_ptr, bstnode *node, int val);
int namesBefore(bstnode *current_ptr, char* name, int val);
void fillArray(customer** arr, bstnode *current_ptr, int* i, int s);

int main(void) {
  //gets user input
  int n;
  scanf("%d",&n);
  bstnode *root=NULL, *temp;

  //asks for commands n times
  for(int i=0;i<n;i++)
    {
      //declare variabales
      char command[14];
      char name[MAXLEN+1];
      int point;

      scanf("%s",command); //gets user input

      //performs command based on user input
      if(strcmp(command, "add")==0)
      {
        scanf("%s %d",name,&point); //gets user input

        //finds node if it exists
        bstnode* found=findNode(root, name);
        bstnode* temp_node=found;

        //checks if node was found
        if(found==NULL)
        {
          temp=create_node(name, point); //creates new node
          
          temp_node=temp; //keeps track of new node
          root=insert(root, temp); //inserts node
        }
        else
        {
          //adds node if customer already exists
          found->custPtr->points+=point; 
        }
        //prints customer's name and points 
        printf("%s %d\n",temp_node->custPtr->name,temp_node->custPtr->points);
        
      }
      else if(strcmp(command, "sub")==0)
      {
        scanf("%s %d",name,&point); //gets user input
        
        //checks if node was found
        if(findNode(root, name)!=NULL)
        {
          bstnode* temp_node=findNode(root, name);
          if(temp_node->custPtr->points>=point)
            {
              (temp_node->custPtr->points)-=point;
            }
            else
              temp_node->custPtr->points=0;
          printf("%s %d\n",temp_node->custPtr->name,temp_node->custPtr->points);
        }
        else
          printf("%s not found\n",name);
        
       
      }
      else if(strcmp(command, "del")==0)
      {
        scanf("%s %d",name,&point);//gets user input
        bstnode* temp_node=findNode(root, name); //find node to delete
        bstnode* end=NULL;//keeps track of parent of deleted node 

        //checks if node was found
        if(temp_node!=NULL)
        {
          root=delete(root, name,&end); //deletes node
          //if parent exists or not
          if(end!=NULL)
          {
            updateSize(root, end); //updates size of previous nodes
          }
          printf("%s deleted\n", name); //prints name of deleted customer
        }
        else
        {
          printf("%s not found\n", name);
        }
      }
      else if(strcmp(command, "search")==0)
      {
        scanf("%s",name);//gets user input
        
        //checks if node was found
        if(findNode(root, name)!=NULL)
        {
          bstnode* temp_node=findNode(root, name);//keeps track of node to find
          
          int val=findHeight(root, temp_node, 0);//finds depth of node
          printf("%s %d %d\n", name, temp_node->custPtr->points,val);//prints name, points, depth
        }
        else
        {
          printf("%s not found\n", name);
        }
      }
      else if(strcmp(command, "count_smaller")==0)
      {
        scanf("%s",name);//gets user input

        int val2=0;//keeps track of names before
        val2=namesBefore(root, name, 0); //finds number of names before
        printf("%d\n", val2);
        
      }
    }
  
  int s=root->size;//finds number of nodes
  //creates customer array and its index
  customer** c=malloc(s*sizeof(customer*));
  int i=0;

  fillArray(c, root, &i,s);//fills array with nodes from BST
  quickSort(c, 0, s-1);//sorts array based on points descending
  
  //prints name and points of customers
  for(int j=0;j<s;j++)
    {
      printf("%s %d\n",c[j]->name,c[j]->points);
    }
  
  //frees tree and nodes in array
  freeTree(root);
  for(int j=0;j<s;j++)
  {
    free(c[j]);
  }
  free(c);

  return 0;
}

void freeTree(bstnode *current_ptr) {

  // Check if there are nodes in the tree.

  if (current_ptr != NULL) {
    //printf("1val: %d\n",val);
    // Found the value at the root.
    freeTree(current_ptr->left); // Go Left.
    freeTree(current_ptr->right); //
    //free(current_ptr->custPtr);
    free(current_ptr);
    
  }
   
  // No node found.
}

void swap(customer** a, customer** b)
{
  char t[MAXLEN+1];
  strcpy(t, (*a)->name);
  int t2=(*a)->points;
  //printf("1points %d %s\n",t2,t);
  strcpy((*a)->name, (*b)->name);
  //printf("4points %d\n",t2);
  (*a)->points=(*b)->points;
  //printf("2points %d\n",t2);
  strcpy((*b)->name, t);
  (*b)->points=t2;
  //printf("3points %d\n",t2);
}
// 1 if a is bigger, o if a is smaller
int comparePoints(customer** a, customer** b)
{
  if((*a)->points!=(*b)->points)
    return (*b)->points-(*a)->points;
  else if((*a)->points==(*b)->points)
    return strcmp((*a)->name,(*b)->name);
}
int partition(customer** a, int left, int right)
{
  int pi=left+rand()%(right-left+1);
  swap(&a[left], &a[pi]);
  pi=left;
  left++;
  //printf("test 1\n");
  while(left<=right)
    {
      while(left<=right && comparePoints(&a[left],&a[pi])<0)
        {
          left++;
        }
        
      while(left<=right && comparePoints(&a[right],&a[pi])>0)
        {
          
          right--;
        }
      
      if(left<=right)
      {
        swap(&a[left],&a[right]);
        
      }
       
    }
    swap(&a[pi],&a[right]);
  
  //printf("test 3\n");
  return right; //return the partition point
}

void quickSort(customer** arr, int low, int high) {

    // Only have to sort if we are sorting more than one number
  //printf("test 4\n");
    if (low < high) {
      //printf("test 5\n");
        int split = partition(arr,low,high);
      
        quickSort(arr,low,split-1);
      
        quickSort(arr,split+1,high);
    }
}
bstnode* insert(bstnode *root, bstnode *element) {

  // Inserting into an empty tree.
  if (root == NULL){
    return element;
  }
  else {

    // element should be inserted to the right.
    // >0 means first string is first
    if (strcmp(element->custPtr->name,root->custPtr->name)>0) {

      // There is a right subtree to insert the node.
      if (root->right != NULL)
      {
        root->right = insert(root->right, element);
      }
      // Place the node directly to the right of root.
      else
        root->right = element;
      root->size++;
    }

    // element should be inserted to the left.
    else {

      // There is a left subtree to insert the node.
      if (root->left != NULL)
        root->left = insert(root->left, element);

      // Place the node directly to the left of root.
      else
        root->left = element;
      root->size++;
    }

    // Return the root pointer of the updated tree.
    return root;
  }
}

bstnode* create_node(char* name, int point) {

  // Allocate space for the node, set the fields.
  bstnode* temp;
  temp = (bstnode*)malloc(sizeof(bstnode));
  temp->custPtr = (customer*)malloc(sizeof(customer));
  //printf("Name:%s, address:%p\n",name,temp->custPtr);
  strcpy(temp->custPtr->name, name);
  temp->custPtr->points=point;
  temp->size=1;
  temp->left = NULL;
  temp->right = NULL;

  return temp; // Return a pointer to the created node.
}

bstnode* findNode(bstnode *current_ptr, char* name) {

  // Check if there are nodes in the tree.
  if (current_ptr != NULL) {

    // Found the value at the root.
    if (strcmp(current_ptr->custPtr->name, name)==0)
      return current_ptr;

    // Search to the left.
    //name is before current_ptr->name
    else if (strcmp(current_ptr->custPtr->name, name)>0)
      return findNode(current_ptr->left, name);

    // Or...search to the right.
    else
      return findNode(current_ptr->right, name);

  }
  else
    return NULL; // No node found.
}

bstnode* parent(bstnode *root, bstnode *node) {

  // Take care of NULL cases.
  if (root == NULL || root == node)
    return NULL;

  // The root is the direct parent of node.
  if (root->left == node || root->right == node)
    return root;

  // Look for node's parent in the left side of the tree.
  if (strcmp(root->custPtr->name, node->custPtr->name)>0)
    return parent(root->left, node);

  // Look for node's parent in the right side of the tree.
  else if (strcmp(root->custPtr->name, node->custPtr->name)<0)
    return parent(root->right, node);

  return NULL; // Catch any other extraneous cases.

}

// Returns a pointer to the node storing the minimum value in the tree
// with the root, root. Will not work if called with an empty tree.
bstnode* minVal(bstnode *root) {

  // Root stores the minimal value.
  if (root->left == NULL)
    return root;

  // The left subtree of the root stores the minimal value.
  else
    return minVal(root->left);
}

// Returns a pointer to the node storing the maximum value in the tree
// with the root, root. Will not work if called with an empty tree.
bstnode* maxVal(bstnode *root) {

  // Root stores the maximal value.
  if (root->right == NULL)
    return root;

  // The right subtree of the root stores the maximal value.
  else
    return maxVal(root->right);
}

// Returns 1 if node is a leaf node, 0 otherwise.
int isLeaf(bstnode *node) {

  return (node->left == NULL && node->right == NULL);
}

// Returns 1 if node has a left child and no right child.
int hasOnlyLeftChild(bstnode *node) {
  return (node->left!= NULL && node->right == NULL);
}

// Returns 1 if node has a right child and no left child.
int hasOnlyRightChild(bstnode *node) {
  return (node->left== NULL && node->right != NULL);
}

bstnode* delete(bstnode* root, char* name, bstnode** end) {

  bstnode *delnode, *new_del_node, *save_node;
  bstnode *par;
  char save_val[MAXLEN+1];
  int save_val2;

  delnode = findNode(root, name); // Get a pointer to the node to delete.

  par = parent(root, delnode); // Get the parent of this node.
  *end=par;
  // Take care of the case where the node to delete is a leaf node.
  if (isLeaf(delnode)) {// case 1

    // Deleting the only node in the tree.
    if (par == NULL) {
      free(root->custPtr);
      free(root); // free the memory for the node.
      return NULL;
    }

    // Deletes the node if it's a left child.
    if (strcmp(par->custPtr->name, name)>0) {
        free(par->left->custPtr);
      free(par->left); // Free the memory for the node. 
      par->left = NULL;
      //par->size--;
      //updateSize(root, par);
    }

    // Deletes the node if it's a right child.
    else {
        free(par->right->custPtr);
      free(par->right); // Free the memory for the node.
      //printf("yooooooooo");
      par->right = NULL;
      //par->size--;
      //updateSize(root, par);
    }

    return root; // Return the root of the new tree.
  }

  // Take care of the case where the node to be deleted only has a left
  // child.
  if (hasOnlyLeftChild(delnode)) {

    // Deleting the root node of the tree.
    if (par == NULL) {
      save_node = delnode->left;
      free(delnode->custPtr);
      free(delnode); // Free the node to delete.
      return save_node; // Return the new root node of the resulting tree.
    }

    // Deletes the node if it's a left child.
    if (strcmp(par->custPtr->name, name)>0) {
      save_node = par->left; // Save the node to delete.
      par->left = par->left->left; // Readjust the parent pointer.
      //par->size--;
      //updateSize(root, par);
      free(save_node->custPtr);
      free(save_node); // Free the memory for the deleted node.
    }

    // Deletes the node if it's a right child.
    else {
      save_node = par->right; // Save the node to delete.
      par->right = par->right->left; // Readjust the parent pointer.
      //par->size--;
      //updateSize(root, par);
      free(save_node->custPtr);
      free(save_node); // Free the memory for the deleted node.
    }

    return root; // Return the root of the tree after the deletion.
  }

  // Takes care of the case where the deleted node only has a right child.
  if (hasOnlyRightChild(delnode)) {

    // Node to delete is the root node.
    if (par == NULL) {
      save_node = delnode->right;
      free(delnode->custPtr);
      free(delnode);
      return save_node;
    }

    // Delete's the node if it is a left child.
    if (strcmp(par->custPtr->name, name)>0) {
      save_node = par->left;
      par->left = par->left->right;
      //par->size--;
      //updateSize(root, par);
      free(save_node->custPtr);
      free(save_node);
    }

    // Delete's the node if it is a right child.
    else {
      save_node = par->right;
      par->right = par->right->right;
      //par->size--;
      //updateSize(root, par);
      free(save_node->custPtr);
      free(save_node);
    }
    return root;
  }
//if your code reaches hear it means delnode has two children
  // Find the new physical node to delete.
  new_del_node = maxVal(delnode->left);
  strcpy(save_val, new_del_node->custPtr->name);
  save_val2 = new_del_node->custPtr->points;
  customer* save_cust=new_del_node->custPtr;
  //new_del_node->size--;

  delete(root, save_val, end);  // Now, delete the proper value.

  // Restore the data to the original node to be deleted.
  strcpy(delnode->custPtr->name,save_val);
  delnode->custPtr->points=save_val2;
  //free(save_cust);
  //delnode->size--;
  //updateSize(root, delnode);
  return root;
}
    
void updateSize(bstnode *current_ptr, bstnode *node) {

  // Check if there are nodes in the tree.

  if (current_ptr != NULL) {
    //printf("1val: %d\n",val);
    // Found the value at the root.
    if (current_ptr==node)
    {
        current_ptr->size--;
      return; 
    }
    // Search to the left.
    //name is before current_ptr->name
    else if (strcmp(current_ptr->custPtr->name, node->custPtr->name)>0)
    {
      //(*val)++;
      //printf("val: %d\n",val);
      updateSize(current_ptr->left, node);
      current_ptr->size--;
    }

    // Or...search to the right.
    else
    {
      updateSize(current_ptr->right, node);
      current_ptr->size--;
    }
  }
  // No node found.
}

int findHeight(bstnode *current_ptr, bstnode *node, int val) {

  // Check if there are nodes in the tree.
  
  if (current_ptr != NULL) {
    // Found the value at the root.
    if (strcmp(current_ptr->custPtr->name, node->custPtr->name)==0)
      return val;

    // Search to the left.
    //name is before current_ptr->name
    else if (strcmp(current_ptr->custPtr->name, node->custPtr->name)>0)
    {
      val++;
      //printf("val: %d\n",val);
      return findHeight(current_ptr->left, node, val);
    }

    // Or...search to the right.
    else
    {
      val++;
      return findHeight(current_ptr->right, node,val);
    }
  }
  else
    return val; // No node found.
}

int namesBefore(bstnode *current_ptr, char* name, int val) {

  //val=findHeight(current_ptr, node, val);
  if (current_ptr != NULL) {
    //printf("1val: %d\n",val);
    // Found the value at the root.
    if (strcmp(current_ptr->custPtr->name,name)==0)
    {
      if (current_ptr->left!=NULL)
        {
          val+=current_ptr->left->size;
        }
      return val;
    }
    // Search to the left.
    //name is before current_ptr->name
    else if (strcmp(current_ptr->custPtr->name,name)>0)
    {
      
      //printf("val1: %d\n",val);
      return namesBefore(current_ptr->left, name, val);
    }
    // Or...search to the right.
    else
    {
      if (current_ptr->left!=NULL)
      {
        val+=current_ptr->left->size;
      }
      val++;
      return namesBefore(current_ptr->right, name,val);
    }
  } 
  
  return val;
}

void fillArray(customer** arr, bstnode *current_ptr, int* i, int s) {

  // Only traverse the node if it's not null.
  if (current_ptr != NULL) {
    fillArray(arr,current_ptr->left,i,s); // Go Left.
     // Print the root.
      arr[(*i)]=current_ptr->custPtr;
      //printf("%s %d\n", arr[(*i)]->name, arr[(*i)]->points);
    if((*i)<s)
      {
      (*i)++;
    }
    fillArray(arr,current_ptr->right,i,s); // Go Right.
  }
  
  //printf("outside: %d\n",*i);
}