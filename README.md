# digital-wallet

Program Summary

1. This program is implemented in C++, which needs support of C++11 to compile.

2. The source file of the program is saved as “antifraud.cpp” under the folder PROJECT_PATH/src.

3. The “run.sh” under PROJECT_PATH is rewritten, which first compiles the source file and generates binary file “antifraud” under the folder PROJECT_PATH/bin, and then executes the binary file to write output files under the folder PROJECT_PATH/paymo_output.

4. The “run_tests.sh” under folder PROJECT_PATH/insight_testsuite remains the same.

5. The organization of PROJECT_PATH directory remains the same. Feel free to add any test case and run PROJECT_PATH/insight_testsuite/run_tests.sh.

6. When running the test, the source file will be first complied and the following warning may happen:
“./src/antifraud.cpp:110:31: warning: range-based for loop is a C++11 extension [-Wc++11-extensions]“.
Just ignore the warnings, which will not affect the running of the program.

7. Summary messages will be printed in the stdout during running, such as the average processing time per payment specified in “stream_payment.txt”, etc.

8. Detailed comments of each function are available in the source file. The core of the program is a query routine which performs an improved breadth-first-search (BFS) to determine whether two id’s are considered trusted friends under a given feature.
