//
//  student_dao.c
//  CProject
//
//  Created by scfhao on 2023/3/25.
//

#include "student_dao.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static sqlite3 *db;

static int ResultSetOffset = 0;

// 数据库文件位置
char *databaseLocation(void) {
    char *dir = "student.sqlite";
    return dir;
}

// 获取数量回调
static int getCountCallback(void *countPtr, int argc, char **argv, char **azColName){
    int *intPtr = (int *)countPtr;
    // 在这个回调函数里，argv[0] 就是要查询的 count(*) 的结果，只不过是字符串形式，通过 sscanf 可以转换到 int 类型
    sscanf(argv[0], "%d", intPtr);
    return 0;
}

// 获取学生列表回调
static int getStudentsCallback(void *studentsArrayPtr, int argc, char **argv, char **azColName){
    // 这是用来保存sql查询结果的数组
    StudentRef students = (StudentRef)studentsArrayPtr;
    // 这是当前回调执行时，要处理的学生结构
    StudentRef stu = students + ResultSetOffset;
    int i;
    for(i=0; i<argc; i++){
        if (strcmp(azColName[i], "name") == 0) {
            strcpy(stu->name, argv[i]);
        } else if (strcmp(azColName[i], "age") == 0) {
            int age;
            int *agePtr = &age;
            sscanf(argv[i], "%d", agePtr);
            stu->age = age;
        } else {
            printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        }
    }
    ResultSetOffset++;
    return 0;
}

// 准备数据库，比如创建表
void prepareDatabase(void) {
    char *errMsg = 0;
    char *sql = "create table student(name text, age int)";
    int rc = sqlite3_exec(db, sql, NULL, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}

void openDatabase(void) {
    if (!db) {
        char *dbPath = databaseLocation();
        // 获取数据库文件是否存在
        int exists = access(dbPath, 0) == 0;
        int rc = sqlite3_open(dbPath, &db);
        if (rc) {
            fprintf(stderr, "Can't open database");
            sqlite3_close(db);
            return;
        }
        if (!exists) {
            // 如果之前数据库文件不存在，执行初始化操作
            prepareDatabase();
        }
    }
}

// 关闭数据库连接
int closeDatabase(void) {
    return sqlite3_close(db);
}

/**
 保存学生信息到数据库
 参数 stu：要保存的学生信息
 返回值：保存成功返回1，失败返回0.
 */
int saveStudent(StudentRef stu) {
    char sql[100];
    char *errMsg;
    sprintf(sql, "insert into student(name, age) values ('%s', %d)", stu->name, stu->age);
    int rc = sqlite3_exec(db, sql, NULL, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    return rc == SQLITE_OK;
}

/**
 获取数据库中的学生总数
 返回值：学生人数
 */
int getStudentCount(void) {
    char *errMsg;
    int count = 0;
    char *sql = "select count(*) from student";
    int rc = sqlite3_exec(db, sql, getCountCallback, &count, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    return count;
}

/*
 获取数据库中所有的学生
 返回值：Student[] 学生数组
 */
StudentRef getAllStudents() {
    ResultSetOffset = 0;
    int count = getStudentCount();
    StudentRef students = (StudentRef)malloc(sizeof(struct Student) * count);
    if (!students) {
        fprintf(stderr, "malloc over flow!\n");
        return NULL;
    }
    char *errMsg;
    char *sql = "select name, age from student";
    int rc = sqlite3_exec(db, sql, getStudentsCallback, students, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    return students;
}
