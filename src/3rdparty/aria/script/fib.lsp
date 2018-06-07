[do
  (= fib 
     (function (n)
               (if (>= n 2)
                 (+ (fib (- n 1)) 
                    (fib (- n 2)))
                 n)))

  (= res (fib 10))
  (print res)]

