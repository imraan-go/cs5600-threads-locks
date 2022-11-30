# Chapter 29


## Question 1
Smallest interval is mciroseconds.

## Question 2
Multiple threads require mroe time.
2 threads:
gcc q2.c  && time ./a.out
Thread 2 -  value: 18552144 
Thread 1 -  value: 20000000 
Took seconds 1.362563 
Final value: 20000000 

real    0m1.365s
user    0m1.927s
sys     0m0.731s

 gcc q2.c  && time ./a.out
Thread 2 -  value: 19637619 
Thread 1 -  value: 19663086 
Thread 4 -  value: 19960247 
Thread 3 -  value: 20000000 
Took seconds 2.091816 
Final value: 20000000 

real    0m2.095s
user    0m4.451s
sys     0m3.171s

## Question 3

**Threshold = 1**

gcc q3.c  && time ./a.out
Thread 0 -  value: 18464658 
Thread 1 -  value: 19416316 
Thread 2 -  value: 19495046 
Thread 3 -  value: 20000000 
Took seconds 2.510542 
Final value: 20000000 

real    0m2.512s
user    0m5.910s
sys     0m3.313s



**Threshold = 5**
gcc q3.c  && time ./a.out
Thread 0 -  value: 17184085 
Thread 3 -  value: 18660270 
Thread 2 -  value: 19984030 
Thread 1 -  value: 20000000 
Took seconds 1.060112 
Final value: 20000000 

real    0m1.061s
user    0m3.235s
sys     0m0.412s



**Threshold = 100**

gcc q3.c  && time ./a.out
Thread 0 -  value: 16436100 
Thread 3 -  value: 17035600 
Thread 2 -  value: 19588500 
Thread 1 -  value: 20000000 
Took seconds 0.903681 
Final value: 20000000 

real    0m0.905s
user    0m3.084s
sys     0m0.010s


## Question 4

## Question 5

## Question 6
