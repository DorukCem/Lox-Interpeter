# Lox Interptreter

The project was made thanks to [Crafting Interpreters](https://www.craftinginterpreters.com/). This is the C++ implementation of the book's language Lox using a tree-walk interpreter.



# How to compile

Use your compilers 'make' command using the makefile provided
```
make
```

# How to use

To run a REPL session: Run main
```
.\main
```
```
>var a = 3;
>var b = 20;
>print a * b;
   60.0
>exit
   terminated
```
use exit to end the REPL session

To run a script: Run main with name of the script
```
.\main example.lox
```

# Example Code

## Classes
```
// base class
class Doughnut {
  cook() {
    print "Fry until golden brown.";
  }
}

// derived class
class BostonCream < Doughnut {
  cook() {
    super.cook();
    print "Pipe full of custard and coat with chocolate.";
  }
}

BostonCream().cook();
```

## Functions
recursion:
```
fun fib(n) {
  if (n <= 1) return n;
  return fib(n - 2) + fib(n - 1);
}

for (var i = 0; i < 20; i = i + 1) {
  print fib(i);
}
```
closures:
```
fun makeCounter() {
  var i = 0;
  fun count() {
    i = i + 1;
    print i;
  }

  return count;
}

var counter = makeCounter();
counter(); // prints "1".
counter(); // prints "2".
```
