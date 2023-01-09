# crisp functional programming language 

crisp is an interpreted functional programming language, inpsired by Lisp. It is implemented in C and currently in version alpha-1.0.0. Any contribution is welcome and appreciated. To understand the language you can check out `programs` directory where there are plenty of examples of how language works, how syntax and grammar is defined etc. 


## Introductory Guide

### Hello World Program

Before we start, you should know that you will be using parenthesis more than you have used ever since. To display a `Hello World` message with Crisp, run the following code.
```scheme
    (print "Hello world")
```
Even now with this code snippet, you probably understood that any function call or any builtin operation will be executed using this format: `([operation name] [..arguments])`.

### Summation & Multiplication and More...

To sum, multiply, subtract or divide, we will again use parenthesis. In fact, there is no escape from parenthesis (I am so sorry).
```scheme
     (+ 5 5)  //  10
     (- 5 5)  //  0
     (* 23 0) //  0
     (/ 0 0)  //  Divide by zero 0 exception will pop up. Learn your math!
     (+ (- 2 2) 5)  // 5
     (* 10 10 10)   // 1000
 ```
You can also use `+` operator on strings to concatenate them:

```scheme
    (print (+ "Everbody " "loves" " crisp")) // prints a fact!
```

### Variables

You can define variables two ways: using `let` or `define`. The difference between those two is that `define` allows you to define variables globally, whereas let excepts you to give the scope of this variable as an expression.

```scheme
(define x 10)

(print x)             // prints 10

(let x 5 (print x))   // prints 5

(print x)             // prints 10
```


 
 
