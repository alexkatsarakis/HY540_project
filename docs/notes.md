# HY-540: Project Notes

* Assume that the following are different elements:
```JavaScript
object[1]

object["1"]
```

* Assume that user functions can be represented by an numeric index (just like in Alpha Compiler). **This might not be valid**. An idea expressed during the lectures is that each function should point to the AST node with the first executable statement.

* Should I use pointers for Value instances? If pointer are not used then it would be a good idea to use pointer to strings in the union for better performance (we don't want to copy the string all the time).