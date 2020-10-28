#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct node
{
  char name[20];
  int id;
  float balance;
  struct node *right;
  struct node *left;
}Node;

Node *GenerateN(char name[], int id ,float bal)
{
  Node *New_N = (Node *)malloc(sizeof(Node));
  strcpy(New_N->name, name);
  New_N->balance = bal;
  New_N->id = id;
  New_N->left=New_N->right = NULL;
  return New_N;
}

Node * minValueNode(Node * root)
{
    Node* current = root;

    /* loop down to find the leftmost leaf */
    while (current && current->left != NULL)
    {
      current = current->left;
    }
      return current;
}

Node *Insert(Node  *tree, Node *tree2)
{

	if (tree == NULL)
  {
    //printf("created root\n");
		return tree2;
	}
	else if (tree->id < tree2->id)
  {
    //printf("moving right\n");
    //printf("\ntree id is :%d\n",tree->id);
    //printf("\n inserting node id2 is :%d\n",tree2->id);
		tree->right = Insert(tree->right, tree2);
	}
	else if(tree->id > tree2->id)
  {
    //printf(" moving left\n");
    //printf("\ntree id is :%d\n",tree->id);
    //printf("\ninserting node id2 is :%d\n",tree2->id);
		tree->left = Insert(tree->left, tree2);
	}
	return tree;
}

Node * search(Node * root,int id)
{
  // Node *found = root;  // this will look for key
   if (root == NULL){
     //not found in tree
    return NULL;
   }
    if(root->id == id)
    { //if id is found in tree
      return root;
    }
    else if(root->id > id)
    {
      return search(root->left,id);
    }
    else
    {
      return search(root->right,id);
    }
  }
Node * readfile(Node *root)
{
    char character;
    char  narr[100], name[100], barr[100];

    // printf("Enter file name to read:");

    // scanf("%s",fname);
    char fname[100] = "customers.txt";
    FILE *fpr;
    int bracesCheck = 0;
    int id, n, b,cc, numb;
    float bal;
    id = 0, n = 0, b = 0, cc = 0;

    fpr = fopen(fname,"r");

    while((character = getc(fpr)) != EOF)
    {
        //printf("Current C: %c \n", character );
        if(character == ',')
        {

          bracesCheck ++;
          //printf("\nmade it here\n");
        }
        else if(isdigit(character) && bracesCheck == 0)
        {
            narr[id++] = character;
            narr[id] = '\0';
        }
        else if((isalpha(character) && bracesCheck == 1) || (character == '.' && bracesCheck == 1))
        {
          name[n++] = character;
          name[n] = '\0';
          //cc++;
        }
        else if((isdigit(character) && ((bracesCheck) == 2)) || (character == '.' && bracesCheck == 2))
        {
          barr[b++] = character;
          barr[b] = '\0';
          //cc++;
        }
        else if(character == '}')
        {
          numb = atoi(narr);
          bal = atof(barr);
          //printf("name: %s\n", name);
          //printf("id: %04d\n", numb);
          //printf("balance: %04f\n", bal);
          if(root == NULL)
          {
            Node *tempN = GenerateN(name,numb, bal);
            root = tempN;
            //printf("\nmade it here once\n");
          }
          else
          {
            //printf("made it here");
            Node *tempN = GenerateN(name,numb, bal);
            Insert(root,tempN);
          }
          //tempN = GenerateN(name,numb,bal);
          //Insert(root,tempN);
          id =0; n= 0; b =0 ,cc =0;
          bracesCheck = 0;
        }
    }
  fclose(fpr);
  return root;
}

void add(Node *root,int id,char name[],int credit)
{ Node * found;
  //printf("\nid is here %d\n",id);
  found = search(root,id);
  // check if id is taken
  if(found == NULL)
    {
      Node * tempN = GenerateN(name,id, credit);
      Insert(root,tempN);
		}
	else
    {
		printf("Error the id [%d] is already in use\n",id);
	  }
}

Node* deleteNode(Node * root,int id)
{
      // base case
    //printf("made it here");
    if (root == NULL)
    {
      //printf("made it here WHY ");
      return root;
    }
    if (root->id > id)
        root->left = deleteNode(root->left, id);

    // If the key to be deleted is greater than the root's key,
    // then it lies in right subtree
    else if (root->id < id)
        root->right = deleteNode(root->right, id);

    // if key is same as root's key, then This is the node
    // to be deleted
    else
    {
        // node with only one child or no child
        if (root->left == NULL)
        {
            Node *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
             Node *temp = root->left;
             free(root);
             return temp;
         }

         // node with two children: Get the inorder successor (smallest
        // in the right subtree)
         Node* temp = minValueNode(root->right);

         // Copy the inorder successor's content to this node
         root->id = temp-> id;
         root->balance = temp->balance;
         strcpy(root->name,temp->name);

         // Delete the inorder successor
         root->right = deleteNode(root->right, temp->id);
    }
     return root;
 }

 void Inorder(Node *root,FILE *fpr3)
 {
 	if (root)
  {
 		Inorder(root->left,fpr3);
 		fprintf(fpr3,"{%04d, %s, %.2f}\n",root->id, root->name, root->balance);
 		Inorder(root->right,fpr3);
 	}
 }
 void Postorder(Node * root, FILE *fpr3)
 {
   if(root)
   {
     Postorder(root->left,fpr3);
     Postorder(root->right,fpr3);
     fprintf(fpr3,"{%04d, %s, %.2f}\n",root->id, root->name, root->balance);
   }
 }
 void preOrder(Node * root,FILE *fpr3)
 {
   if(root)
   { //printf("The Pre order is:\n");
     fprintf(fpr3,"{%04d, %s, %.2f}\n",root->id, root->name, root->balance);
     preOrder(root->left,fpr3);
     preOrder(root->right,fpr3);
   }
 }

void printScreen(Node * customer, float owed)
{
  if (customer->balance < owed)
  {
    printf("Customer %d, %s, payed $%.2f from their store balance and $%.2f at the register, %.2f remaining store balance\n",customer->id,customer->name,customer->balance,owed-customer->balance,0.00);
    customer->balance = 0.00;
  }
  else if(customer->balance > owed)
  {
   printf("Customer %d, %s, payed $%.2f from their store balance and $%.2f at the register, %.2f remaining store balance\n",customer->id,customer->name,owed,0.00,customer->balance-owed);
   customer->balance = customer->balance-owed;
  }
  else if(customer->balance == owed)
  {
    printf("Customer %d, %s, payed $%.2f from their store balance and $%.2f at the register, %.2f remaining store balance\n",customer->id,customer->name,customer->balance,0.00,0.00);
    customer->balance = 0.00;
  }
}

void process(Node * root, char fileN[])
{
  FILE *fpr2;
  int cust_id=0;
  float cust_bal=0;
  int ic=0,ib=0,cc=0, len; //counters
  //printf("\na%sb\n",fileN);
  len = strlen(fileN);
  if( fileN[len-1] == '\n' )
  {
    fileN[len-1] = 0;
    fileN[strlen(fileN)]='\0';
  }
  //printf("\na%sb\n",fileN);
  char character2, arrid[10],arrbal[10],character3[40];
  fpr2 = fopen(fileN,"r");
  while((character2 = getc(fpr2)) != EOF)
  {
    if(character2 == ',')
    {
       cc++;
    }
    else if(isdigit(character2) && cc == 0)
    {
      arrid[ic++] = character2;
      arrid[ic] = '\0';
    }
    else if((isdigit(character2) && cc == 1) || character2 == '.')
    {
      arrbal[ib++] = character2;
      arrbal[ib] = '\0';
    }
    else if(character2 == '}')
    {
      cust_id = atoi(arrid);
      cust_bal = atof(arrbal);
      //printf("this is cust id %d and bal is %.2f",cust_id,cust_bal);

      Node * found = search(root,cust_id);
      if(found != NULL)
      {

        printScreen(found, cust_bal);
      }
      ib = 0; cc = 0;
      ic = 0;
    }
  }
  fclose(fpr2);
}

void WriteToF(Node * root,char input[])
{
  FILE *fpr3;
  fpr3 = fopen("newcustomers.txt","w+");
  if(strcmp("pre",input)== 0)
        {
          preOrder(root,fpr3);
        }
        else if(strcmp("post",input)== 0)
        {
          Postorder(root,fpr3);
        }
        else if(strcmp("in",input)== 0)
        {
          Inorder(root,fpr3);
        }
  fclose(fpr3);
}

int main()
{
  Node *root = NULL; char str[100], garb[10],name[15],fname[20],pick[3]; int temp_id; float temp_bal; char *str2;
  //int x;  char temp_name[20], temp_menu[10],garb[10], temp_file[20];
  //int temp_id = 0; float temp_bal = 0;
  root = readfile(root);
  while(str[0] != 'q'){
    printf("1)Add in the format <add num name balance>\n2)Delete in the format <delete id>\n3)Process in the format <Process filename>\n4)Quit with format <quit>\nEnter here: ");
    fgets(str,100,stdin);
    if(str[0] == 'a')
    {
      str2 = strtok(str," "); strcpy(garb,str2);
      str2 = strtok(NULL," "); temp_id = atoi(str2);
      str2 = strtok(NULL," "); strcpy(name,str2);
      str2 = strtok(NULL," "); temp_bal = atof(str2);
      add(root,temp_id,name,temp_bal);
    }
    else if(str[0] == 'd')
    {
      str2 = strtok(str," "); strcpy(garb,str2);
      str2 = strtok(NULL," "); temp_id = atoi(str2);
      printf("id is %d",temp_id);
      deleteNode(root,temp_id);
    }
    else if (str[0] == 'p')
    {
      str2 = strtok(str," "); strcpy(garb,str2);
      str2 = strtok(NULL," ");
      strcpy(fname,str2);
      process(root,fname);
    }
    else if (str[0] == 'q')
    {
      printf("please enter output formart: pre, post, or in order:");
      scanf("%s",pick);
      WriteToF(root,pick);
      printf("writing to newcustomers.txt in %s-order, shutting down...",pick);
    }
  }
  return 0;
}
