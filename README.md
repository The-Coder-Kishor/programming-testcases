# programming-testcases
Testcases for projects and assignments
Go to the folder having your main.c and matrix.c
```
git clone -b cp-5 --single-branch https://github.com/The-Coder-Kishor/programming-testcases
mv ./programming-testcases/* ./
rm -rf programming-testcases
```
Then provide file permissions:
```
chmod u+x ./validator.sh
```
To run the script:
```
./validator.sh
```

If you have valgrind installed on your system, you can run the following comprehensive script to test for valgrind errors too:
```
chmod u+x ./valg_output.sh
./valg_output.sh
```