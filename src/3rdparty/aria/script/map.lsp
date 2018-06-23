(do
  (random-seed)

  (= m (map)) 
  ; or (= m (map <optional> <key> <value>)) 
  ; <optioanl> map's max size
  ; <key> string 
  ; <value> any value,eg: vector, list, float 

  (= i 0)
  (dotimes 100000
	(= val (string (chr (random 50 88)) (chr (random 50 128)) "l" (chr (random 50 58))))
	(map-add m val i)
	(= i (+ i 1))
	)
  (map-remove m "Ja17")
  (map-remove m "Jal18")
  (print (map-get m "Jal2"))
)

