#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STACK_SIZE 100

typedef char element;		// ��ü!
// ���Ŀ� ������ �ʿ��ϸ� ���⸸ �����Ͽ� ���δ�. 
// ===== ���� �ڵ��� ���� ===== 
#define MAX_STACK_SIZE 100


typedef struct {
	element data[MAX_STACK_SIZE];
	int top;
} StackType;

// ���� �ʱ�ȭ �Լ�
void init_stack(StackType* s)
{
	s->top = -1;
}

// ���� ���� ���� �Լ�
int is_empty(StackType* s)
{
	return (s->top == -1);
}
// ��ȭ ���� ���� �Լ�
int is_full(StackType* s)
{
	return (s->top == (MAX_STACK_SIZE - 1));
}
// �����Լ�
void push(StackType* s, element item)
{
	if (is_full(s)) {
		fprintf(stderr, "���� ��ȭ ����\n");
		return;
	}
	else s->data[++(s->top)] = item;
}
// �����Լ�
element pop(StackType* s)
{
	if (is_empty(s)) {
		fprintf(stderr, "Invalid expression\n");
		exit(1);
	}
	else return s->data[(s->top)--];
}
// ��ũ�Լ�
element peek(StackType* s)
{
	if (is_empty(s)) {
		fprintf(stderr, "���� ���� ����\n");
		exit(1);
	}
	else return s->data[s->top];
}
// ===== ���� �ڵ��� �� ===== 

// �������� �켱������ ��ȯ�Ѵ�.
int prec(char op)
{
	switch (op) {
	case '(': case ')': return 0;
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	}
	return -1;
}
// ���� ǥ�� ���� -> ���� ǥ�� ����
char* infix_to_postfix(char exp[])
{
	int i = 0;
	int k = 0;
	char ch, top_op;
	int len = strlen(exp);
	StackType s;
	char* out = (char*)malloc(sizeof(char) * (len + 1)); // ���� �޸� �Ҵ�

	init_stack(&s);
	for (i = 0; i < len; i++) {
		ch = exp[i];
		switch (ch) {
		case '+': case '-': case '*': case '/': // ������
			// ���ÿ� �ִ� �������� �켱������ �� ũ�ų� ������ ���
			while (!is_empty(&s) && (prec(ch) <= prec(peek(&s))))
			{
				out[k++] = pop(&s); // out �迭�� ����
			}

			push(&s, ch);
			break;
		case '(':   // ���� ��ȣ
			push(&s, ch);
			break;
		case ')':   // ������ ��ȣ
			top_op = pop(&s);
			// ���� ��ȣ�� ���������� ���
			while (top_op != '(') {
				out[k++] = top_op; // out �迭�� ����
				top_op = pop(&s);
			}
			break;
		default:        // �ǿ�����
			out[k++] = ch; // out �迭�� ����
			break;
		}
	}
	while (!is_empty(&s)) {
		out[k++] = pop(&s); // out �迭�� ����
	}
	out[k] = '\0'; // �������� NULL ���� ����

	return out;
}

int check_matching(const char* in)
{
	StackType s;
	char ch, open_ch;
	int i, n = strlen(in);  	// n= ���ڿ��� ����
	init_stack(&s);			// ������ �ʱ�ȭ

	for (i = 0; i < n; i++) {
		ch = in[i];		// ch = ���� ����
		switch (ch) {
		case '(':   case '[':    case '{':
			push(&s, ch);
			break;
		case ')':   case ']':    case '}':
			if (is_empty(&s))  return 0;
			else {
				open_ch = pop(&s);
				if ((open_ch == '(' && ch != ')') ||
					(open_ch == '[' && ch != ']') ||
					(open_ch == '{' && ch != '}')) {
					return 0;
				}
				break;
			}
		}
	}
	if (!is_empty(&s)) return 0; // ���ÿ� ���������� ����
	return 1;
}

int eval(char exp[])
{
	int op1, op2, value, i = 0;
	int len = strlen(exp);
	char ch;
	StackType s;

	init_stack(&s);
	for (i = 0; i < len; i++) {
		ch = exp[i];
		if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
			op2 = pop(&s);
			op1 = pop(&s);
			switch (ch) { //������ �����ϰ� ���ÿ� ���� 
			case '+': push(&s, op1 + op2); break;
			case '-': push(&s, op1 - op2); break;
			case '*': push(&s, op1 * op2); break;
			case '/': push(&s, op1 / op2); break;
			}

			
		}
		else if (ch >='0'&& ch <='9') {	//�������̸� �ǿ����ڸ� ���ÿ��� ����
			value = ch - '0';	// �Է��� �ǿ������̸�
			push(&s, value);
		}
		else {
			fprintf(stderr, "Invalid character\n");
			exit(1);
		}
	}
	return pop(&s);
}

// 
int main(void)
{
	char s[100];
	printf("Enter an infix expression : ");
	scanf_s("%s", s,sizeof(s));
	
	if (check_matching(s) != 1)
	{
		fprintf(stderr, "Mismatched parentheses\n");
		exit(1);
	}
	char* post = infix_to_postfix(s);
	eval(post);
	printf("Postfix expression : %s\n",post);
	
	
	
	printf("result : %d", eval(post));

	return 0;
}