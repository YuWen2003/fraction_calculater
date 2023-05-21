#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 20

/* 定義分數結構 */
typedef struct frac_oper{
    int numerator;
    int denominator;
    char operand;
}frac_oper;

/* 反轉string */
void reverse(frac_oper *str, int str_len){
    int i, j;
    frac_oper c;
    for(i=0, j=str_len-1; i<j; ++i, --j)
        c = str[i], str[i] = str[j], str[j] = c;
}

/* clear data */
void clear_data(frac_oper *d){
    for(int i=0; i<MAX; i++){
        d[i].numerator = 0;
        d[i].denominator = 0;
        d[i].operand = '\0';
    }
}

/* 輸入方程式 */
int input_data(frac_oper *d){
    char input[MAX];
    int i=0, j=0, k=0, m=0, n=0, p=0, data_len = 0;
    clear_data(d);

    while(scanf("%s", &input)){
        if(input[0] == '=')
            break;
        else if(input[0]=='+' || input[0]=='-' || input[0]=='*' || input[0]=='/')
            d[i].operand = input[0];
        else{
            j=strlen(input);
            while(input[0] == '('){
                d[i].operand = '(';
                i++;
                for(k=1; input[k]!='\0'; k++)
                    input[k-1] = input[k];
                input[j-1] = '\0';
                j--;
            }
            p=0;
            while(input[j-1] == ')'){
                p++;
                input[j-1] = '\0';
                j--;
            }
            for(k=0; input[k]!='/'; k++);
            m = 1;
            n = k;
            while(--n>=0){
                d[i].numerator+=(input[n]-'0')*m;
                m*=10;
            }
            m = 1;
            n = j;
            while(--n>k){
                d[i].denominator+=(input[n]-'0')*m;
                m*=10;
            }
            while(p--){
                i++;
                d[i].operand = ')';
            }
        }

        memset(input, '\0', MAX);
        i++;
    }
    return i;
}

/* compare */
int compare(char op){
    switch(op){
        case '+': case '-':
            return 1;
        case '*': case '/':
            return 2;
        default:
            return 0;
    }
}

/* 將中序改前序 */
int infix_to_prefix(frac_oper *str, frac_oper *new_str, int str_len){
    frac_oper stack[MAX];
    clear_data(new_str);
    clear_data(stack);
    int top=0, j=0, i, new_str_len=str_len;

    for(i=str_len-1; i>=0; i--){
        switch(str[i].operand){
            case '+': case '-': case '*': case '/':
                while (compare(str[i].operand) < compare(stack[top].operand))
                    new_str[j++] = stack[top--];
                stack[++top] = str[i];
                continue;
            case ')':
                stack[++top] = str[i];
                new_str_len--;
                continue;
            case '(':
                while (stack[top].operand != ')')
                    new_str[j++] = stack[top--];
                top--;
                new_str_len--;
                continue;
            default:
                new_str[j++] = str[i];
                continue;
        }
    }
    while(top != 0){
        new_str[j++] = stack[top--];
    }
    reverse(new_str, new_str_len);

    return new_str_len;
}

/* 找最大公因數 */
int GCD(int t,int v){
    if(t%v==0)  return v;
    else    return GCD(v,t%v);
}

/* 進行運算並約分 */
frac_oper get_value(frac_oper op1, frac_oper op2, char op){
    frac_oper value;
    int k;
    value.operand = '\0';

    switch(op){
        case '+':
            value.numerator = op1.numerator * op2.denominator + op2.numerator * op1.denominator;
            value.denominator = op1.denominator * op2.denominator;
            break;
        case '-':
            value.numerator = op1.numerator * op2.denominator - op2.numerator * op1.denominator;
            value.denominator = op1.denominator * op2.denominator;
            break;
        case '*':
            value.numerator = op1.numerator * op2.numerator;
            value.denominator = op1.denominator * op2.denominator;
            break;
        case '/':
            value.numerator = op1.numerator * op2.denominator;
            value.denominator = op1.denominator * op2.numerator;
            break;
    }
    k = GCD(value.numerator, value.denominator);
    value.numerator/=k;
    value.denominator/=k;

    return value;
}

/* 利用前序表示式進行運算 */
frac_oper Calculation(frac_oper *str, int str_len) {
    frac_oper stack[MAX];
    clear_data(stack);
    int top = 0, j = 0, i;

    for(i=str_len-1; i>=0; i--){
        switch(str[i].operand){
            case '+': case '-': case '*': case '/':
                stack[top-1] = get_value(stack[top], stack[top-1], str[i].operand);
                top--;
                continue;
            default:
                stack[++top] = str[i];
                continue;
        }
    }

    if(stack[top].denominator < 0){
        stack[top].numerator*=-1;
        stack[top].denominator*=-1;
    }
    return stack[top];
}

int main() {
    int infix_len=0, prefix_len=0, j;
    struct frac_oper infix[MAX], prefix[MAX], ans;

    infix_len = input_data(infix);
    /*
    for(j=0; j<infix_len; j++){
        if(infix[j].operand!='\0')
            printf("%c ", infix[j].operand);
        else
            printf("%d/%d ", infix[j].numerator, infix[j].denominator);
    }
    printf("= \n");
    */

    prefix_len = infix_to_prefix(infix, prefix, infix_len);
    /*
    for(j=0; j<prefix_len; j++){
        if(prefix[j].operand!='\0')
            printf("%c ", prefix[j].operand);
        else
            printf("%d/%d ", prefix[j].numerator, prefix[j].denominator);
    }
    printf("= ");
    */

    ans = Calculation(prefix, prefix_len);
    printf("%d/%d\n", ans.numerator, ans.denominator);

    return 0;
}

/*THIS CODE IS MY OWN WORK. IT WAS WRITTEN WITHOUT CONSULTING CODE WRITTEN BY OTHER STUDENTS OR ANY ONE WHO IS NOT AWARE OF MY REFERENCE. 410125034 陳俞妏*/
