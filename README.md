# myCasLock
cas lock
```
gcc -O2 lthread_mutex_test.c -lpthread -o lthread_mutex_test
gcc -O2 pthread_mutex_test.c -lpthread -o pthread_mutex_test
```


# test
my centos7(vmware)

```
#time ./lthread_mutex_test 
counter: 300000000
real    0m1.785s
user    0m1.942s
sys     0m1.347s


#time ./pthread_mutex_test    
counter: 300000000
real    0m7.170s
user    0m8.224s
sys     0m4.472s
```
