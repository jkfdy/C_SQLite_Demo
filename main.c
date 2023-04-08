#include <stdio.h>
#include "student_dao.h"
#include <stdlib.h>

void inputStudentInfo(StudentRef student) {
    printf("请输入姓名：");
    scanf("%s", student->name);
    getchar();
    printf("请输入年龄：");
    scanf("%i", &student->age);
}

int main() {
    // 打开数据库连接
    openDatabase();
    while (1) {
        char op;
        printf("是否继续录入？Y/n\n");
        scanf("%c", &op);
        // 如果输入 Y/y/回车 则继续录入学生信息，否则录入结束
        if (op != 'Y' && op != 'y' && op != '\n') {
            break;
        }
        
        Student stu;
        // 从控制台读取学生信息
        inputStudentInfo(&stu);
        // 将读取的学生信息保存到数据库
        saveStudent(&stu);
    }
    // 录入结束，查出数据库中保存的学生信息并打印
    printf("以下是数据库中保存的学生信息：\n");
    int count = getStudentCount();
    StudentRef students = getAllStudents();
    for (int i=0; i<count; i++) {
        logStudentInfo(students+i);
    }
    // 释放 students 所占内存
    free(students);
    // 关闭数据库连接
    closeDatabase();
    return 0;
}
