#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Course {
    int id;
    char *name;
};

struct Course *make_course(int id, char *name) {
    struct Course *new_course = malloc(sizeof(struct Course));
    new_course->id = id;
    new_course->name = name;
    // new_course->name = malloc(sizeof(char) * 100);
    // strcpy(new_course->name, name);
    return new_course;
}
int main() {
    struct Course *cs161 = make_course(161, "Computer Security");
    // strcpy(cs161->name, "Operating Systems");
    printf("Welcome to CS%d: %s!\n", cs161->id, cs161->name);
    return 0;
}
