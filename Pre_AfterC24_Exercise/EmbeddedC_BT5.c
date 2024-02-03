#include <stdio.h>
#include <stdint.h>
//typedef
struct infor
{
    char name[32];
    char mssv[8];
    char course_c[2];
};
//prototype
void infor_input (struct infor* stu_infor);
void infor_print (struct infor* stu_infor);
//declare variable

int main()
{
    struct infor stu_infor;
    infor_input(&stu_infor);
    infor_print(&stu_infor);
    return 0;
}
//declare function
void infor_input (struct infor* stu_infor)  //input function
{
    printf("Input name: ");
    gets(stu_infor->name);
    printf("Input MSSV: ");
    gets(stu_infor->mssv);
    printf("Input course_c: ");
    scanf("%d", stu_infor->course_c);
}

void infor_print (struct infor* stu_infor)  //print the information
{
    printf("-------------------------------");
    printf("\nYour name is: ");
    puts(stu_infor->name);
    printf("Your MSSV is: ");
    puts(stu_infor->mssv);
    printf("Congratulation you for finishing C%d course!!", *stu_infor->course_c);
}