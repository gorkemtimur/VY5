
/*
		Name:Ahmet Görkem Timur
		Student ID:15011052
		Infix To Postfix Conversion and evaluation Postfix expressions using Stack 
		
*/
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define SIZE 30

typedef struct
{
	int st[SIZE];
	int top;	
}STACK; 
STACK stack; //I have set it globally

typedef struct
{
	int values[SIZE];
	char variables[SIZE];
	int cnt;
}VARIABLE; // The struct that contains variables and their values.

VARIABLE var; // I have set it globally.

void stackInitialize(); 
void variableInitialize();
int isEmpty();
int isFull();
void push(char); // push function for converting infix to postfix.
void intPush(int); //push function for calculating postfix.
char pop(); // same as top row.
int intPop(); // same as top row.
int Priority(char); //the function that compares transaction priorities.
void InfixToPostfix(char [],char []);
void CalculatePostfix(char [],char []);
void printStack(int); // printfStack(0) uses at InfixToPostfix functions, printfStack(1) uses at CalculatePostfix. 
void printValue(); 


int main()
{
	variableInitialize();
	char *tkn, string[50], variable[5], infix[SIZE], postfix[SIZE]; 
	//string[50]-> file priorities, tkn-> strtok functions, variable[5] -> to store the value of the variable on each line
	int j;
	FILE *f0=fopen("input.txt","r");	
	if(f0==NULL)
	{
		printf("File can not open.");
		exit(1);
	}
	while(!feof(f0))
	{
		for(j=0;j<SIZE;j++)
		{
			infix[j]=NULL;
		}
		fgets(string,50, f0);
		printf("\nReading string: %s\n",string);
		tkn = strtok(string, " ;"); // tkn has the value that is equalized.
		strcpy(variable,tkn); //Stored the value that is equalized.
		var.variables[var.cnt]=tkn[0];
		tkn= strtok(NULL, " ;"); //passing '=' symbol
		j=0;
		while (tkn= strtok(NULL," ;"))
		{
			if(j==0 && isdigit(*tkn)) //if only value exists on the line.
			{
				var.values[var.cnt]=atoi(tkn);
				var.cnt++;
			}
			strcat(infix,tkn);
			j++;
		}
		infix[j]='\0';

		InfixToPostfix(infix,postfix);
		CalculatePostfix(postfix,variable);

	}
		fclose(f0);
		
	
	return 0;
}
void InfixToPostfix(char infix[], char postfix[])
{
	char ch;
	int i, j, k=0; //i->infix iterator, k->postfix iterator
	stackInitialize();
	for(i=0;i<SIZE;i++)
	{
		postfix[i]=NULL;
	}
	for(i=0; infix[i] != '\0'; i++)
	{
		ch = infix[i];
		
		if (isalnum(ch)) //if is a letter or number, i have added postfix.
		{
			postfix[k++] = ch;
		}
		else if (ch == '(')
		{
			push(ch);
		}
		else if (ch == ')')
		{
			while(stack.st[stack.top] != '(') //if ch is ')', stack is emptied until '('
			{
				postfix[k++] = pop();
			}
			pop();
		}
		else if (ch == '/' || ch == '*' || ch == '+' || ch == '-')
		{
			if (Priority(ch) >= Priority(stack.st[stack.top])) //compares between transactions.
			{
				if (Priority(ch) == Priority(stack.st[stack.top])) // if variables have same priority (e.g. '+' and '-' or '*' and '/')
				{													//first the transition will be emptied, then another transition will be pushed.
					postfix[k++] = pop();
				}
				push(ch);
			}
			else
			{
				while (Priority(ch) <= Priority(stack.st[stack.top]))
				{
					postfix[k++] = pop();
				}
						
			}
		}
		printf("Postfix:");
		for(j=0; j<k; j++)
		{
			printf("%c",postfix[j]);
		}		
		printf("\t\t\tStack: ");
		printStack(0);
			printf("\n");
	}
	postfix[k] = '\0';
	while(stack.top > -1)
	{
		postfix[k++] = pop();
		printf("Postfix: %s\t\t\t",postfix);
		printf("Stack:");
		printStack(0);
		printf("\n");
	}
	printf("\nThe postfix result: %s\t",postfix);
	printf("\n");

}

void CalculatePostfix(char postfix [],char variable[])
{
	int i=0, j, value1, value2; //i->postfix iterator, j->the struct variables' iterator, value1 and value2 for calculation while pop struct.
	if(strstr(postfix,"+")==NULL && strstr(postfix,"-")==NULL && strstr(postfix,"*")==NULL && strstr(postfix,"/")==NULL)
	{ // if a variable equals a number on the line.
		printValue();
	}
	else
	{
		stackInitialize();
		while(postfix[i]!='\0')
		{

			if(postfix[i]!='+' && postfix[i]!='-' && postfix[i]!='*' && postfix[i]!='/') //if the values is not transaction symbols
			{	
				if(isdigit(postfix[i]))//if the values is already number
				{
					intPush(postfix[i]-'0');
				}
				else
				{
					j=0;
					while(postfix[i]!=var.variables[j]) //will be found a value that equal to the variable at postfix[i].
						j++;
					intPush(var.values[j]); 
				}
				printf("Stack: ");
				printStack(1);
				printf("\n");		
			}
			else
			{
				value1=intPop();
				value2=intPop();
				switch (postfix[i]) //if a variable is a transaction symbol.
            	{
             		case '+': intPush(value2 + value1); break;
             		case '-': intPush(value2 - value1); break;
             		case '*': intPush(value2 * value1); break;
             		case '/': intPush(value2/value1);   break;
            	}
				printf("Stack: ");
				printStack(1);
				printf("\n");
			}
		i++;
		}
		i=0;
		while(postfix[i]!=variable[0] && postfix[i]!='\0')
			i++;
		if(postfix[i]==variable[0]) //if an old variable is to be assigned a new value.
		{
			var.values[i]=intPop();
		}
		else
		{
		var.variables[var.cnt]=variable[0]; //a new variable is to be assigned a result.
		var.values[var.cnt]=intPop();
		var.cnt++;
		}
		printValue();
		
	}
		
}


int isEmpty()
{
	if(stack.top>-1)
		return (1);
	else 
		return (0);
}
int isFull()
{
	if(stack.top < SIZE-1)
		return (1);
	else 
		return (0);
}
void push(char el)
{
	if (isFull()==1)
	{
		stack.st[++stack.top] = el;
	}
	else
	{
		printf("\nError! Stack is full.\n");
		exit(-1);
	}
}
void intPush(int el)
{
	if(isFull()==1)
	{
		stack.st[++stack.top]= el;
	}
	else
	{
		printf("\nError! Stack is full.\n");
		exit(-1);
	}
}
char pop()
{
	char ch;
	if (isEmpty()==1)
	{
		ch = stack.st[stack.top];
		stack.st[stack.top] = '\0';
		stack.top--;
	
		return ch;
	}
	else
	{
		printf("\nError! Stack is empty.\n");
		exit(-1);
	}
}
int intPop()
{
	int el;
	if (isEmpty()==1)
	{
		el = stack.st[stack.top];
		stack.st[stack.top] = NULL;
		stack.top--;
	
		return el;
	}
	else
	{
		printf("\nError! Stack is empty.\n");
		exit(-1);
	}
}
int Priority(char ch)
{
	if(ch == '(' || ch == ')')
		return 0;
	else if (ch == '+' || ch == '-')
		return 1;
	else if (ch== '/' || ch == '*')
		return 2; 
}
void stackInitialize()
{
	int i;		
	for(i=0; i<SIZE; i++)
	{
		stack.st[i] = '\0';
	}
	stack.top=-1;
}
void variableInitialize()
{
	int i;
	var.cnt=0;
	for(i=0;i<SIZE;i++)
	{
		var.variables[i]='\0';
	}
}
void printStack(int test)
{
	int j;
	if(test==0)
	{
		for(j=0;j<=stack.top;j++)
		{
			printf("%c ",stack.st[j]);
		}	
	}else
		for(j=0;j<=stack.top;j++)
		{
			printf("%d ",stack.st[j]);
		}
	{
		
	}
}
void printValue()
{
	int i;
	printf("\n");
	for(i=0;i<var.cnt;i++)
	{
		printf("%c=>%d\t",var.variables[i],var.values[i]);
	}
	printf("\n");
}
