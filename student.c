//
//  student.c
//  CProject
//
//  Created by scfhao on 2023/3/25.
//

#include "student.h"

void logStudentInfo(StudentRef stu) {
    printf("[Student]--%s--%d\n", stu->name, stu->age);
}
