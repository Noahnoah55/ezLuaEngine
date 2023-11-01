function _update()
    drawRect(x, 100, 20, 20, {255, 255, 255})
    x = x + 1
    if x > 600 then
        x = 0
    end
end

x = 0