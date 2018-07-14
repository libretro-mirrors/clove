(do
	; Shows the basics of looping in Aria.

	#| Known issue 1. (for vector-push as well)
	(= test (list))
	(dotimes ((= n 0) 10)
	  (= test (append test n))) ; it will fill the list with number '10' instead of 0..9
	(print test)
	|#
	#| Solution for issue 1.
	(= test (list))
	(dotimes ((= n 0) 10)
	  (= test (append test (+ n 0))))
	(print test)
	|#


	(= a (list "1" "2" "3" (list "a" "d") (vector "m" "i" "h") 4 5 "six"))
	(= b (vector 1 2 3 4 "a" "b" (list "v" "l") "c" "d"))

	(print "###### Loop 1) ######")
	(dotimes ((= n 0) 5)
		(print n " yes"))

	#| NOTE: loop 2 will be faster than 3 |#

	(print "###### Loop 2) ######")
	(dotimes ((= n 0) (length a))
	  (= current (nth a n))

	  (when (eq (type current) 'pair)
		(print "Found pair, lets get its components: " (car current) " " (car (cdr current))))

	  (print (type current) current n))

	(print "###### Loop 3) ######")
	(= cpy_a a) ; You have to have a copy of the list you want to iterate throught else you mutate its value
	(loop (not (endp cpy_a))
		(print (type (car cpy_a)) (car cpy_a))
		(= cpy_a (cdr cpy_a)))

	(print "###### Loop 4) ######")
	(dotimes ((= n 0) (vector-length b))
	  (print (type (vector-get b n)) (vector-get b n)))


	(print "###### Loop 5) ######")
	(dovector ((= n 'it) b)
			  (print (type it))
			  (print it))


	(print "###### Loop 6) ######")
	(dolist ((= n 'it) a)
	  (print (type it) it))

)
