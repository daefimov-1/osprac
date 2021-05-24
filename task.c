#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

void addnum(char* s, int index) {
    s[index] += 1;
    if (s[index] > 'z') {
        s[index] = 'a';
        addnum(s, index - 1);
    }
}

int main()
{
    int     fd;
    char    fileName[] = "task.c";
    char    symlinkName[] = "test/aa";
    (void) umask(0);

    // директория для файлов
    if (mkdir("test", 0777) < 0) {
        printf("Can\'t create dir 'test'. If it exists, please delete it.\n");
        exit(-1);
    }

    // начальный файл
    if ((fd = open("test/aa", O_WRONLY | O_CREAT, 0666)) < 0) {
        printf("Can\'t open file\n");
        exit(-1);
    }
    // проверка на закрытие
    if (close(fd) < 0) {
        printf("Can\'t close file\n");
    }

    int maxRecursion = 0; // максимальная рекурсия
    while (1) {
        addnum(symlinkName, 6);
        if (symlink(fileName, symlinkName) != 0) {
            printf("Can\'t create symlink\n");
            exit(-1);
        }

        // если не можем открыть файл, значит мы достигли максимального уровня рекурсии
        if ((fd = open(symlinkName, O_WRONLY, 0666)) < 0) {            
            break;
        } else {
            maxRecursion++;
        }

        if (close(fd) < 0) {
            printf("Can\'t close file\n");
        }

        addnum(fileName, 1);
    }

    printf("maxRecursion = %d\n", maxRecursion);


    return 0;
}
