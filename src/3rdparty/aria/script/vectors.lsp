(do 
  #| Simple example on how to use vectors in Aria |#
  ; Lets declare a null vector 
  (= my-vector (vector)) 
  (print my-vector)
  ; Lets push some values to this vector 
  (vector-push my-vector "hi")
  (vector-push my-vector (vector 1 2 3 4 "hi from another vector" 3))
  (vector-push my-vector (list "10" "hello" 42))
  (print "We have added these elements: " my-vector)
  ; Lets change the element 0("hi") to something else,
  (vector-set my-vector 0 "Lisp")
  (print my-vector "has length: " (vector-length my-vector))
  ; Lets remove the last element  
  (vector-pop my-vector)
  (print my-vector "has length: " (vector-length my-vector))
  ;Get a value from an index 
  (print (vector-get my-vector 3))
  ;Check if a value exists in our vector 
  (print (vector-find my-vector 3))
  (print (vector-member my-vector 3))
  (print (vectorp my-vector))
)
