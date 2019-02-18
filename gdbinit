echo set sysroot ~/buildroot/output/staging \n
echo target extended-remote 10.0.0.185:10001 \n
echo remote put problem3hw2 problem3hw2 \n 
echo set remote exec-file problem3hw2 \n
file problem3hw2
echo b main \n
echo problem3hw2.c:8\n
echo problem3hw2.c:13\n
echo start \n
echo n \n
echo n \n
echo n \n
echo n \n
echo n \n
