a = [
    { "one" : 2 }
];
b = [
    { "one" : 2 }
];
a = b;
print(a == b);
function foo(){};
a = foo;
print(a == foo);

i = 0;
while(true){
    if(i > 100){
        break;
    }
    print(i);
    i++;
}
a = 5;
a = a+++3;
print(a);
if(a != 6){
    print("a");
}else{
    print("b");
}
print("foo" + "bar");




function print(arg){
    console.log(arg);
}
