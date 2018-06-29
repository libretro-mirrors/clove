(do
 ; This example shows you how to make a "class"(structure like in C, really it's just a map) using Aria
(= my-structure (map
    "frames" (list 1 2 3 4 5)
    "image" "image.png"
    "size" (list 32 32)
    "color" (vector 255 100 200 255)
   ))

  ;(print "C like structure:" my-structure)

  (= color (map-get my-structure "color"))
  (print "color" color (type color))
  ; Lets modify the color
  (= color (vector 0 0 0 0))
  (map-add my-structure "color" color)
  (print "New color" (map-get my-structure "color"))

  (print "Get image: " (map-get my-structure "image"))

  (= foo
   (function (cls)
        (print "Get size: " (map-get cls "size"))
    ))

  (foo my-structure)

  )
