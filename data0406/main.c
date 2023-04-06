#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STACK_SIZE 100

typedef char element;		// 교체!
// 차후에 스택이 필요하면 여기만 복사하여 붙인다. 
// ===== 스택 코드의 시작 ===== 
#define MAX_STACK_SIZE 100


typedef struct {
	element data[MAX_STACK_SIZE];
	int top;
} StackType;

// 스택 초기화 함수
void init_stack(StackType* s)
{
	s->top = -1;
}

// 공백 상태 검출 함수
int is_empty(StackType* s)
{
	return (s->top == -1);
}
// 포화 상태 검출 함수
int is_full(StackType* s)
{
	return (s->top == (MAX_STACK_SIZE - 1));
}
// 삽입함수
void push(StackType* s, element item)
{
	if (is_full(s)) {
		fprintf(stderr, "스택 포화 에러\n");
		return;
	}
	else s->data[++(s->top)] = item;
}
// 삭제함수
element pop(StackType* s)
{
	if (is_empty(s)) {
		fprintf(stderr, "Invalid expression\n");
		exit(1);
	}
	else return s->data[(s->top)--];
}
// 피크함수
element peek(StackType* s)
{
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else return s->data[s->top];
}
// ===== 스택 코드의 끝 ===== 

// 연산자의 우선순위를 반환한다.
int prec(char op)
{
	switch (op) {
	case '(': case ')': return 0;
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	}
	return -1;
}
// 중위 표기 수식 -> 후위 표기 수식
char* infix_to_postfix(char exp[])
{
	int i = 0;
	int k = 0;
	char ch, top_op;
	int len = strlen(exp);
	StackType s;
	char* out = (char*)malloc(sizeof(char) * (len + 1)); // 동적 메모리 할당

	init_stack(&s);
	for (i = 0; i < len; i++) {
		ch = exp[i];
		switch (ch) {
		case '+': case '-': case '*': case '/': // 연산자
			// 스택에 있는 연산자의 우선순위가 더 크거나 같으면 출력
			while (!is_empty(&s) && (prec(ch) <= prec(peek(&s))))
			{
				out[k++] = pop(&s); // out 배열에 저장
			}

			push(&s, ch);
			break;
		case '(':   // 왼쪽 괄호
			push(&s, ch);
			break;
		case ')':   // 오른쪽 괄호
			top_op = pop(&s);
			// 왼쪽 괄호를 만날때까지 출력
			while (top_op != '(') {
				out[k++] = top_op; // out 배열에 저장
				top_op = pop(&s);
			}
			break;
		default:        // 피연산자
			out[k++] = ch; // out 배열에 저장
			break;
		}
	}
	while (!is_empty(&s)) {
		out[k++] = pop(&s); // out 배열에 저장
	}
	out[k] = '\0'; // 마지막에 NULL 문자 삽입

	return out;
}

int check_matching(const char* in)
{
	StackType s;
	char ch, open_ch;
	int i, n = strlen(in);  	// n= 문자열의 길이
	init_stack(&s);			// 스택의 초기화

	for (i = 0; i < n; i++) {
		ch = in[i];		// ch = 다음 문자
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
	if (!is_empty(&s)) return 0; // 스택에 남아있으면 오류
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
			switch (ch) { //연산을 수행하고 스택에 저장 
			case '+': push(&s, op1 + op2); break;
			case '-': push(&s, op1 - op2); break;
			case '*': push(&s, op1 * op2); break;
			case '/': push(&s, op1 / op2); break;
			}

			
		}
		else if (ch >='0'&& ch <='9') {	//연산자이면 피연산자를 스택에서 제거
			value = ch - '0';	// 입력이 피연산자이면
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