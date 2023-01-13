(define total 0)

(for i 100 
    (begin
        (if (|| (= i 50) (= i 75))
            (begin
                (print i " is here!")
                (define total (+ total i))
            )
        ) 
    )
)

(print total)