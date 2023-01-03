(let name (read) 
          (let content (read) 
                       (let f (open name "w") 
                       (begin
                       (write f content)
                       (close f)
                       ))))