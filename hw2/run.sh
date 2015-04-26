make clean 2&> /dev/null
make vm 
./vm -t 100 apps/hello.so
make clean 2&> /dev/null
