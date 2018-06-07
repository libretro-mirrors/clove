
(do
  #| This file is used to test various aspects from Aria. |#
  (random-seed)
  
  (= assert (function (n)
	  (if (eq n nil) (print "Assert error at: " n) (print "Correct"))))

	(= a "a string")
	(cond (eq a "a string") (print "Correct.")
		  (< 0 10) (print "Correct")
		  (< 10 20) (print "Correct")
		  (eq (mod 4 2) 0) (print "Correct"))
	
	(assert (numberp 1))
	(assert (stringp "str" "str"))
	(assert (consp (cons 1 2)))

	(= a 100)
	(dotimes 50 (when a > 50) (= a (- a 1)))
	(assert (not (eq a 0)))
    (print a)

	#|
	Testing comments |#
	(print "Correct")
	
	(print "Max between 10 and 20 " (max 10 20))
	(print "Min between 10 and 20 " (min 10 20))
	(print "Random value between 0 and 100 " (random 100))

	#| 
		Testing multi-line comments 
	|# 
	(print "Correct")
	
	; Testin single line comment 
	(print "Correct")

	#|Testing comments|#
	(print "Correct")
		
	(print "Calling from custom module")
    (cond 
      (eq (uname) "Linux") (= lib (import "./script/my_lib.so"))
      (eq (uname) "Apple") (= lib (import "./script/my_lib.dylib"))
      (eq (uname) "Windows") (= lib (import "./script/my_lib.dll")))
    (print (callme))
    (assert (not (eq callme 42)))
		
    ;After you're done using the library you have to close the library and free memory
    (free-import lib)
	[print "12 tests passed"]
)
