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
    if getKey("Space") then
        table.insert(particles, {px=x, py=y, vx=math.random(-5,5), vy=math.random(-20,5)})
    end

    drawSquare(x, y, 10, 10)
    for k, v in pairs(particles) do
        drawSquare(v.px, v.py, 10, 10)
        particles[k].vy = v.vy + 2
        particles[k].px = v.vx + v.px
        particles[k].py = v.vy + v.py
        if particles[k].py > 300 then
            particles[k] = nil
        end
    end
end

x = 50
y = 50
spd = 3

particles = {}