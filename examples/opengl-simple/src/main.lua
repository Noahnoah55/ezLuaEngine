function _update()
    drawRect(0, 0, x, 300, 0, {255-math.floor((x/600)*255), 255, 0})
    x = x + 2
    if x > 600 then
        x = 0
    end
    drawRect(295, 100, 10, 100, math.rad(x)*3, {0, 0, 255})
end

x = 0