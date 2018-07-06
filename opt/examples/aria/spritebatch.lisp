(do
  (= img (love:graphics-newImage "image.png"))
  (= batch (love:graphics-newSpriteBatch img 30000))

  (love:graphics-spritebatch-clear batch)
  (untill 30000
    (love:graphics-spritebatch-add batch (random 0 900) (random 0 900)))

  (= font (love:graphics-newFont 12))

  (= love-load
   (function ()))

  (= love-update
    (function ()))

  (= love-draw
   (function ()
        (love:graphics-drawSpriteBatch batch)
        (love:graphics-setColor 255 0 0 255)
        (love:graphics-print font (string "FPS: " (love:timer-getFPS)) 100 100)
        (love:graphics-setColor 255 255 255 255)
  ))

  (= love-quit
    (function ()
        (love:graphics-spritebatch-free batch)
    ))

  )
