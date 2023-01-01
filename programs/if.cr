(let eq (function x y (= x y))
(let a 1
(let b 2
(if (eq a b)
    (print a " is " b)
    (print a " is not " b)
)
)))