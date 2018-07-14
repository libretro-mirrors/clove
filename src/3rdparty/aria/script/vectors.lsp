(do
  #| Simple example on how to use vectors in Aria |#
  ; Lets declare a null vector
  (= my-vector (vector))
  (print my-vector)
  ; Lets push some values to this vector
  (vector-push my-vector "hi")
  (vector-push my-vector (vector 1 2 3 4 "hi from another vector" 3))
  (vector-push my-vector (list "10" "hello" 42))
  (vector-push my-vector 1997)
  (print "We have added these elements: " my-vector)
  ; Lets change the element 0("hi") to something else,
  (vector-set my-vector 0 "Lisp")
  ; Lets remove the last element
  (vector-pop my-vector)
  ;Get a value from an index
  (print "value at index 0: " (vector-get my-vector 0))
  (print "value at index 1: " (vector-get my-vector 1))
  ;Check if a value exists in our vector
  (print my-vector)
  (vector-push my-vector 1997)
  (print (vector-find my-vector 0 1997))
  (print (vectorp my-vector))
)
