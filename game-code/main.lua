function _update()
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

    drawSquare(x, y, 10, 10)
end

x = 50
y = 50
spd = 3

print("test!")