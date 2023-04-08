//
//  student_dao.h
//  CProject
//
//  Created by scfhao on 2023/3/25.
//

#ifndef student_dao_h
#define student_dao_h

#include <stdio.h>
#include "student.h"

void openDatabase(void);
// 关闭数据库连接
int closeDatabase(void);

/**
 保存学生信息到数据库
 参数 stu：要保存的学生信息
 返回值：保存成功返回1，失败返回0.
 */
int saveStudent(StudentRef stu);

/**
 获取数据库中的学生总数
 返回值：学生人数
 */
int getStudentCount(void);

/*
 获取数据库中所有的学生，这个数组使用后记得释放内存
 返回值：Student[] 学生数组
 */
StudentRef getAllStudents(void);

#endif /* student_dao_h */
