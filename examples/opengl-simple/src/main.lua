function _update()
    drawSpr(0, 0, x, 300, 0, "ezle.png")
    x = x + 2
    if x > 600 then
        x = 0
    end
    drawSpr(295, 100, 10, 100, math.rad(x)*3, "ezle.png")
end

x = 0