# HY-540: Project Notes

* Assume that the following are different elements:
```JavaScript
object[1]

object["1"]
```

* Should we use pointers for Value instances? If pointers are not used then it would be a good idea to use pointer to strings in the union for better performance (we don't want to copy the strings all the time).