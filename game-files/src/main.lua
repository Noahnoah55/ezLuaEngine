package.path = "game-files/src/?.lua;" .. package.path
require("two")

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

    drawRect(x-1,y-1, 18, 18)
    drawImage("game-files/assets/ezle.png", x, y, 1, 1)
    for k, v in pairs(particles) do
        drawRect(v.px, v.py, 10, 10)
        particles[k].px = v.vx + v.px
        particles[k].py = v.vy + v.py
    end
end

x = 50
y = 50
spd = 3

particles = {}