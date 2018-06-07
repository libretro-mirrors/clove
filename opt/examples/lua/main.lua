require("model_viewer")

function love.load(arg)
    WINDOW_W = love.graphics.getWidth()
    WINDOW_H = love.graphics.getHeight()

    love.graphics.setBackgroundColor(30, 30, 30, 255)

    font = love.graphics.newFont(14)
    love.graphics.setFont(font)
    
    img1 = love.graphics.newImage("example.png")
    img2 = love.graphics.newImage("car.png")

    viewer = model_viewer.create(img1, true)
    car = model_viewer.create(img2, true, 64, 64) 
end

function love.update(dt)
    
    if love.keyboard.isDown("esc") then
        love.event.quit()
    end
end

function love.draw()
    love.graphics.print("FPS:"..love.timer.getFPS(),25,15)
    
    viewer:draw(240,240)
    car:draw(100,240)
end

