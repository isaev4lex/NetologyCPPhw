#include "first_task.h"
#include "second_task.h"
#include "third_task.h"

int main() {
    //first_task_main();
    //second_task_main();
    third::third_task_main();
    return 0;
}

// Я заменил методы print_info на show_info в третьем задании, чтобы они не мешали друг другу, т.к. во втором задании есть такой-же метод.
// Я объеденил всё в один файл, ради удобства написания кода и проверки кода. ( Мы это ещё не проходили, но я знаю что в пайтоне так можно, а значит
// в С++ тоже так можно )