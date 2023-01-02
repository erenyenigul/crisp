(define fibb 
    (function x
        (if (< x 2)
            1
            (+ (fibb (- x 1)) (fibb (- x 2)))
        )   
    )
)

(print (fibb 30))