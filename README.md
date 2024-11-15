
<p align="center">
    Федеральное государственное автономное образовательное учреждение высшего образования  
    <br>«Национальный исследовательский университет ИТМО»<br>
    Факультет программной инженерии и компьютерной техники
</p>
<br>
<h1 align="center">Отчёт</h1>
<p align="center">
    по первой лабораторной работе<br>
    по дисциплине «Операционные системы»
</p>
<br>
<p align="right">
    Выполнил: Нягин М.А.<br>
    Группа P3313<br>
    Преподаватель: Смирнов В. И.
</p>

<br>
<p align="center">
    Санкт-Петербург<br>
    ~ 2024 ~
</p>

# Задание
Платформа:
**Linux**
ForkKind
**vfork**
Нагрузчики:
1. ema-sort-int - Сортировка массива чисел во внешней памяти
2. factorize - Разложение числа на простые множители
### Инструкция по запуску
1. Сборка докера
```shell
docker build -t lab1 . && docker run -it lab1
```
2. Внутри докера программы уже скомпилированы и запускаются в следующем формате:

Первая программа: `factorize`: Разложение числа на простые множители
```shell
./factorize <число для разложения на множетил> <число запусков программы>
```
example:
```shell
./factorize 10 2

Iteration 1: 2 5 
Iteration 2: 2 5 
Factorization completed.
```

Вторая программа: `ema-sort-int`: Сортировка массива чисел во внешней памяти
```shell
./ema-sort-int <file_for_writing_random_files> <sorted> <num_elements>
```
emaple:
```shell
./ema-sort-int inputWhereWillBeWrittenRandomNum sortedOutput 10
External memory sorting completed. Sorted data saved to sortedOutput

root@4905410134af:/app# cat inputWhereWillBeWrittenRandomNum 
9383
886
2777
6915
7793
8335
5386
492
6649
1421

root@4905410134af:/app# cat sortedOutput 
492
886
1421
2777
5386
6649
6915
7793
8335
9383
```

Третья программа: оба нагрузчика запускаются через потоки
```shell
./combined <number_to_factorize> <iterations> <num_elements_for_sorting>
```

example:
```shell
root@4905410134af:/app# ./combined 10 1 10
Iteration 1: 2 5 
Factorization completed.
Sorting completed. Data saved to sorted_output.txt
All tasks completed.
root@4905410134af:/app# 
```

!todo "добавить копию отчета"