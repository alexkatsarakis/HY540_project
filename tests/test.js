x = 10;


function foo(){}

y = file_open("test.txt","w+");
file_write(y,"test");
print(file_read(y)+" from file");
file_close(y);