(define l (list))

(for i 10 
    (begin
        (push l i)
        (print "The list is expanded to: " l)
    )
)

(insert l true 0)
(print l)
(pop l 1)
(print l)

(let a l (print "a and l are references to the same list! " a))

(print "Last element of the list is popped : " (pop l))
(print "no")
(print "The list is now : " l)