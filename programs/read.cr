(let f (open "../todo" "r")
    (define contents (read f))
)

(print contents)