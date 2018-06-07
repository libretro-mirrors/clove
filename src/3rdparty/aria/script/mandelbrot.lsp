(do

  ; load lib
  (eval (parse (read "lib.lsp") "lib.lsp") global)

  (= width 79)
  (= height 44)
  (= chars " .,;/oO%8@#")

  (times height (function (y)
    (print (join (collect (function (p)
      (times width (function (x)
        (let (x0 (+ -2.2 (* (/ 4 width) x))
              y0 (+ -1.5 (* (/ 3 height) y))
              x  0
              y  0
              n  16
              i  1
              z  nil)
          (loop (and (< (+ (* x x) (* y y)) 4) (< i n))
            (= z (+ (- (* x x) (* y y)) x0)
               y (+ (* 2 x y) y0)
               x z
               i (+ i 1)))
          (= z (* (/ i n) (- (strlen chars) 1)))
          (p (substr chars z 1))))))))))))
