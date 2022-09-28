local scale = 5
local hue = 0
local increment = -1

local led_array = leds.new()
local size = led_array:size()

function main()
    for i = 1, size do
        led_array:setHue(i, hue + (i * scale))
    end

    hue = hue + increment
    if hue < 0 then
        hue = 255
    elseif hue > 255 then
        hue = 0
    end
    
    leds.showLeds()
end