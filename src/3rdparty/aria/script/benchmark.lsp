(do
  #|
  (= a (map))
  (= n 0)
  (dotimes (n 1000000)
		(map-add a (string "a" n) (+ n 0)))
  |#
  (= a (vector))
  (= n 0)
  (dotimes (n 3)
	(vector-push a 'a)
	(vector-push a (list 'b 'c))
    )
  (print "GOT" a "VALUE" (vector-get a 1) (type (vector-get a 1)) "LEN" (vector-length a ))

  (= c 0)
  (= n 0)
  (= b (vector 0 0 0 0 0))
  (vector-set b 0 1)
  (print "B" b)

  #|(loop (< n 1000000)
		(vector-push a n)
		(= n (+ n 1)))|#
  )
