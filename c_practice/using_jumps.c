#include<stdio.h>
#include<setjmp.h>
#include<signal.h>

static jmp_buf bufferA;
static jmp_buf bufferB;

void nested_function(int A) {
	if (A == 0) { 
		printf("Can't do that...\n");
		longjmp(bufferA, 1);
	}
}

void div_function(int number) {
	nested_function(number);
	printf("the answer is %d\n", number/5);
}

void main(void)
{
	setjmp(bufferA);
	int num;
	printf("Give a number: ");
	scanf("%d", &num);
	div_function(num);
}


