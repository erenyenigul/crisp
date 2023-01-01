(define money 100)

(define spend 
        (function amount 
                (define money (- money amount))
        )
)

(define earn 
        (function amount 
                (define money (+ money amount))
        )
)

(define info
        (function
                    (print (+ "You have ") money " dollars")
        )
)