(define readFile 
        (function filePath x
            (let f (open filePath "r")
                    (let content (read f)
                            (begin
                                (close f)
                                content
                            )
                    )
            )
        )
)

(define writeFile 
        (function filePath content
            (let f (open filePath "w")
                    (begin
                        (write f content)
                        (close f)
                    )
            )
        )
)