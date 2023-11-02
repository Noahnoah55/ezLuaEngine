function _update()
    drawRect(x, 150, 300, 150, {x, 0, 0})
    x = x + 1
    if x > 255 then
        x = 0
    end
end

x = 0