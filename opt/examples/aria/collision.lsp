[do 
  (love:window-setTitle "Collision test")

  
  (= entity-get-x (function (v) (vector-get v 0)))
  (= entity-get-y (function (v) (vector-get v 1)))
  (= entity-get-w (function (v) (vector-get v 2)))
  (= entity-get-h (function (v) (vector-get v 3)))
  (= entity-get-vel-x (function (v) (vector-get v 4)))
  (= entity-get-vel-y (function (v) (vector-get v 5)))
  (= entity-get-drag-x (function (v) (vector-get v 6)))
  (= entity-get-drag-y (function (v) (vector-get v 7)))


  (= entity-set-x (function (v val) (vector-set v 0 val)))
  (= entity-set-y (function (v val) (vector-set v 1 val)))
  (= entity-set-w (function (v val) (vector-set v 2 val)))
  (= entity-set-h (function (v val) (vector-set v 3 val)))
  (= entity-set-vel-x (function (v val) (vector-set v 4 val)))
  (= entity-set-vel-y (function (v val) (vector-set v 5 val)))
  (= entity-set-drag-x (function (v val) (vector-set v 6 val)))
  (= entity-set-drag-y (function (v val) (vector-set v 7 val)))


  (= entity-init 
     (function (x y w h velx vely dragx dragy)
                 (when (endp velx) (= velx 0))
                 (when (endp vely) (= vely 0))
                 (when (endp dragx) (= dragx 0))
                 (when (endp dragy) (= dragy 0))
                 (vector x y w h velx vely dragx dragy)))

  (= entity-aabb 
     (function (v1 v2)
        (let (x-col y-col ret)
          (= x-col [ and 
                     [> (+ (entity-get-x v1) (entity-get-w v1)) (entity-get-x v2)]     
                     [< (entity-get-x v1) (+ (entity-get-x v2) (entity-get-w v2))]])
          (= y-col [ and 
                [> (+ (entity-get-y v1) (entity-get-h v1)) (entity-get-y v2)]     
                [< (entity-get-y v1) (+ (entity-get-y v2) (entity-get-h v2))]])
    
          (return (and x-col y-col)))))

  (= entity-calculatemtd 
     (function (v1 v2) 
               (let ( mtd-x  mtd-y 
                      v1minx v1miny
                      v2minx v2miny
                      v1maxx v1maxy
                      v2maxx v2maxy
                      left right up down ret)
                 (= v1minx (entity-get-x v1))
                 (= v1miny (entity-get-y v1))
                 (= v2minx (entity-get-x v2))
                 (= v2miny (entity-get-y v2))

                 (= v1maxx (+ (entity-get-x v1) (entity-get-w v1)))
                 (= v1maxy (+ (entity-get-y v1) (entity-get-h v1)))
                 (= v2maxx (+ (entity-get-x v2) (entity-get-w v2)))
                 (= v2maxy (+ (entity-get-y v2) (entity-get-h v2)))
    

                 (= left (- v2minx v1maxx))
                 (= right (- v2maxx v1minx))
                 (= up (- v2miny v1maxy))
                 (= down (- v2maxy v1miny))

                 (= mtd-x 0)
                 (= mtd-y 0)

                 ; When this check returns true there's no collision 
                 (unless (or (> left 0) (< right 0) (> up 0) (< down 0))

                 ; Determine the collision on the x and y axis
                 (if (< (abs left) right) 
                   (= mtd-x left)
                   (= mtd-x right))


                 (if (< (abs up) down)
                   (= mtd-y up)
                   (= mtd-y down))


                 (if (< (abs mtd-x) (abs mtd-y))
                   (= mtd-y 0)
                   (= mtd-x 0))

                 (vector mtd-x mtd-y))
                 )))

  (= entity-collision 
     (function (v1 v2)
               (let (coll hor ver mtd)
                 (= coll (entity-aabb v1 v2))
                 
                 (= mtd nil)
                 (when coll 
                   (= mtd (entity-calculatemtd v1 v2)))

                 ;(print "MTD: " mtd (type mtd))

                [ when mtd 
                  (unless (zerop (vector-get mtd 0))
                    (= hor t))
                  (unless (zerop (vector-get mtd 1))
                    (= ver t))]

                (when (and hor mtd)
                  (entity-set-x v1 (+ (entity-get-x v1) (vector-get mtd 0)))
                  (entity-set-vel-x v1 0))

                (when (and ver mtd)
                  (entity-set-y v1 (+ (entity-get-y v1) (vector-get mtd 1)))
                  (entity-set-vel-y v1 0))
               )))

  (= entity-calculate-drag 
     (function (v velx vely)
               (let 
                 (dragx dragy)
                 (= dragx (entity-get-drag-x v))
                 (= dragy (entity-get-drag-y v))

                 (when (> velx 0)
                   (= velx (- velx (* dragx (love:timer-getDelta))))
                   (when (< velx 0)
                     (= velx 0)))

                 (when (< velx 0)
                   (= velx (+ velx (* dragx (love:timer-getDelta))))
                   (when (> velx 0)
                     (= velx 0)))

                 (when (> vely 0)
                   (= vely (- vely (* dragy (love:timer-getDelta))))
                   (when (< vely 0)
                     (= vely 0)))

                 (when (< vely 0)
                   (= vely (+ vely (* dragy (love:timer-getDelta))))
                   (when (> vely 0)
                     (= vely 0)))

                 (vector velx vely)
                 )))

  ; Apply this on dynamic objects
  (= entity-update-position 
     (function (v)
               (entity-set-x v (+ (entity-get-x v) (entity-get-vel-x v)))
               (entity-set-y v (+ (entity-get-y v) (entity-get-vel-y v)))))


  (= e1 (entity-init 200 140 32 32 0 0 1.4 1.4))
  (= e2 (entity-init 200 240 32 32))
  (= e3 (entity-init 232 240 32 32))

    
  (= font (love:graphics-newFont 12))

  (= velx 0 vely 0)
  (= speedx 100 speedy 100)
  (loop (not (or (love:event-quit) (eq (love:keyboard-keypressed) (ord "q"))))
        (love:graphics-update)
        (love:timer-update)
        (love:keyboard-update)
    
        (love:graphics-print font 
                             (string "FPS: " (love:timer-getFPS) " velx " velx " vely " vely)
                             20 30)
        (love:graphics-rectangle "fill" (entity-get-x e1) (entity-get-y e1) 
                                 (entity-get-w e1) (entity-get-h e1))
        (love:graphics-rectangle "fill" (entity-get-x e2) (entity-get-y e2) 
                                 (entity-get-w e2) (entity-get-h e2))
        (love:graphics-rectangle "fill" (entity-get-x e3) (entity-get-y e3) 
                                 (entity-get-w e3) (entity-get-h e3))


        ; Calculate drag 
        (= drag (entity-calculate-drag e1 velx vely))
        (= velx (vector-get drag 0))
        (= vely (vector-get drag 1))
       
        (when (eq (love:keyboard-keypressed) (ord "d"))
          (= velx (* (+ velx speedx) (love:timer-getDelta))))
        (when (eq (love:keyboard-keypressed) (ord "a"))
          (= velx (* (- velx speedx) (love:timer-getDelta))))
        (when (eq (love:keyboard-keypressed) (ord "s"))
          (= vely (* (+ vely speedy) (love:timer-getDelta))))
        (when (eq (love:keyboard-keypressed) (ord "w"))
          (= vely (* (- vely speedy) (love:timer-getDelta))))
          
       
        (entity-set-vel-x e1 velx)
        (entity-set-vel-y e1 vely)

        (entity-update-position e1)

        (entity-collision e1 e2)
        (entity-collision e1 e3)

        (love:graphics-swap))
  ]

