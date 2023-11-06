function _update()
    drawSpr(0, 0, x, 300, 0, "ezle.png")
    x = x + 2
    if x > 600 then
        x = 0
    end
    drawSpr(px, py, 10, 100, math.rad(x)*3, "ezle.png")
    if getKeyHeld("w") then
        py = py - spd
    end
    if getKeyHeld("a") then
        px = px - spd
    end
    if getKeyHeld("d") then
        px = px + spd
    end
    if getKeyHeld("s") then
        py = py + spd
    end
end

px = 295
py = 100
spd = 3
x = 0