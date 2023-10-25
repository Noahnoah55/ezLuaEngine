function _update()
    if t == 0 then
        playMusic("assets/title3.ogg")
    end
    t = t + 1
    if getKey("w") then
        y = y - spd
    end
    if getKey("s") then
        y = y + spd
    end
    if getKey("a") then
        x = x - spd
    end
    if getKey("d") then
        x = x + spd
    end
    if getKey("j") then
        scale = scale * 1.1
    end
    if getKey("k") then
        scale = scale * .9
    end
    if getKeyJustPressed("space") then
        playSound("assets/pluck.wav")
    end

    drawRect(x-1,y-1, 18, 18, {255, x, y})
    drawImage("assets/ezle.png", x, y, scale, scale)
    drawLine(0,0,x,y,{255,255,255})
    drawText("Hello world!", 600-x, 300-y, 25, {255, 255, 255}, "assets/Roboto/Roboto-Regular.ttf")
    drawText("t = " .. tostring(t), 10, 0, 25, {255, 255, 255}, "assets/Roboto/Roboto-Regular.ttf")
end
t = 0

scale = 1

x = 50
y = 50
spd = 3

particles = {}