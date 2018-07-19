(do
  #| This file is used to test various aspects from Aria. |#

  (random-seed)

  (= a "a string")
  (cond (eq a "a string") (print "Correct.")
        (< 0 10) (print "Correct")
        (< 10 20) (print "Correct")
        (eq (mod 4 2) 0) (print "Correct"))

  (assert (numberp 1))
  (assert (stringp "str" "str"))
  (assert (consp (cons 1 2)))

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
  (print "Correct") ; test

  #|
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
  |#

  [print "12 tests passed"]

  (= a (vector -2 (list 3) (vector 2 3) 42 (list 1243 55) (list 2222 22)))
  (print a (vector-length a))
  (= b (vector))
  (vector-push b a)
  (vector-push b 1997)
  (vector-push b (list 4 5) )
  (vector-push b "vlad")
  (vector-push b (vector 99 90))
  (vector-push b "vlad")

  (= index 0)
  (loop (not (endp (vector-find b index "vlad")))

        (= index (vector-find b index "vlad"))
        (print index)
        (= index (+ index 1))
        )


  (= a (list (list 2 3)))
  (print a)
  (= b (list))
  (= b (append b a))
  (= b (append b 1997))
  (= b (append b 4))
  (= b (append b "hey"))
  (= b (append b (vector 4 5)))
  (= b (append b (list 4 5)))
  (= b (append b 4))
  (= b (append b "vlad"))
  (= b (append b "hey"))

  (print "Done " (member b "hey") "  " (member b 4))
  (= b (remove b "vlad"))
  (= b (remove-n b "hey" 1))


  (= my-class-members
     (function (x)

        ; GLOBAL FUNCTION!
        (= my-class-member-1
           (function (x)
             (print (* x (+ x 10)))
             ))

        ; LOCAL FUNCTION!
        (let (mother nil)
        (= mother
           (function (duck)
             (print "Hello from local function! " duck)
             )))
        (= ret "return!")
     ))


  (= my-class (map))
  (map-add my-class "init" (my-class-members 2))
  (print my-class)

  (print "From hello.lsp:")
  (require "script/hello.lsp")

  (= temp (vector 5 6 7))
  (= t2 (vector 1 2 3 temp))
  (vector-push t2 temp)

  (print t2)

  (= my-structure (map
    "frames" (list 1 2 3 4 5)
    "image" "image.png"
    "size" (list 32 32)
    "color" (vector 255 100 200 255)
   ))

  (print "C like structure:" my-structure)

  (= color (map-get my-structure "color"))
  (print "color" color (type color))
  ; Lets modify the color
  (= color (vector 0 0 0 0))
  (map-add my-structure "color" color)
  (print "New color" (map-get my-structure "color"))


  (print (eq 'test "test")) ; symbol != string

  (= size 0)
  (= color (vector "testing-tests" 3 4 5))
  (dotimes ((= i 0) (vector-length color))
		  (print (type (vector-get color i)))
		  (if (eq (type (vector-get color i)) 'string)
		  (= size (+ size (strlen (vector-get color i))))
		  (= size (+ size (sizeof (vector-get color i))))))
  (print size)

 ; Lets read serialized and deserialize it

  (= rez (substr (read "script/read_from_file.txt") 0 10))
  (print (eval (parse rez "rez")))

  (= rez (substr (read "script/read_from_file.txt") 10 20))
  (print (eval (parse rez "rez")))


  (= rez (substr (read "script/read_from_file.txt") 20))
  (print (eval (parse rez "rez")))

  (if+ (and (>= 2 1) (not (zerop 1)))
	   ((print "yes condition 1"))
	   ((print "no condition 1")
	   (print "no condition 2"))
	   )

  #|
  (= func-foo
	 (function (a b)

			   ))
  |#

  (= default-opt-text "carrots are healthy")
  (= test -3)
  (defn default-value-function
		(a b (c default-opt-text) (d test))
        (print a b c d)
  )

  (default-value-function 1 2)
  (print "defn has type of: " (type default-value-function))


  (= ten 10)
  (= my-vector (vector ten 1 '2 "3" (list 4 5) (vector 7 8)))
  (print "BEFORE" my-vector)
  (vector-remove my-vector 4)
  (print "AFTER" my-vector)
  (print ten)
  )
