(do 
  (= img (love:graphics-newImage "image.png"))
  (= batch (love:graphics-newSpriteBatch img 30000))

  (love:graphics-spritebatch-clear batch)
  (for-inc (= i 0) 30000
    (love:graphics-spritebatch-add batch (random 0 900) (random 0 900)))

  (= font (love:graphics-newFont 12))

  (loop (not (love:event-quit)) 
        (love:graphics-update)
        (love:timer-update)
        (love:keyboard-update)

        (love:graphics-drawSpriteBatch batch)
        (love:graphics-print font (string "FPS: " (love:timer-getFPS)) 100 100)
        (love:graphics-swap)))
