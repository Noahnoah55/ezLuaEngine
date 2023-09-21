print("[LUA] Hello from lua!")

key_enter = 40

function _update()
    if getKey(key_enter) == 0 then
        drawSquare(10,20,30,40)
    end
end