(do
  (random-seed)
  (= m (map)) 
	
  (dotimes ((= i 0) 100000)
	(map-add m (string (chr (random 32 100) ) "l" (chr (random 50 120))) (random 1 2) ))

  (= dope "dope")
  (= m (map)) 
  (map-add m "test" 33)
  (map-add m "test" dope)
  (map-add m "tset" 11)
  (map-add m "testt" 1)
  (map-add m "testt" 2)
  (map-add m "testt" (vector 1 2 'tt ))
  (print (map-get m "testt"))
  (print (map-get m "tset"))
  (print (map-get m "test"))
)

