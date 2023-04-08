//
//  student.h
//  CProject
//
//  Created by scfhao on 2023/3/25.
//

#ifndef student_h
#define student_h

#include <stdio.h>

/**
 Student 结构体定义
 */
typedef struct Student {
    char name[10]; /* 姓名 */
    int age;    /* 年龄 */
} Student, * StudentRef;

// 打印输出学生信息到控制台
void logStudentInfo(StudentRef stu);

#endif /* student_h */
