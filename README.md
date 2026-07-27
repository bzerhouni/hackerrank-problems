# Hackerrank problems

This is a repository where I try to solve some hackerrank problems.

### Sources for the problems :

The description of the problems are copied in each problem's README. The set of problems solved are taken from these 
blogs :
- [https://www.hackerrank.com/blog/c-plus-plus-interview-questions/](https://www.hackerrank.com/blog/c-plus-plus-interview-questions/)

### Structure

Each problem's resources are stored in a specific directory (eg. [cpp/attribute_parser](cpp/attribute_parser)). The
structure of each problem's directory is as follows :

```
cpp/
    | ...
    |-> problem/
        |-> test_cases/       # some of the input test cases defined in .txt files (most of hackerrank problems rely on stdin input)
        |-> CMakeLists.txt    # cmake targets for the problem's solution
        |-> main.cpp          # most hackerrank problems are to be solved only in a main.cpp file
        |-> README.md         # a copy of the problem's description
        |-> run_test_cases.sh # a script that builds the solution and runs it on available test cases
```
