local led_array = leds.new()
local size = led_array:size()

local fadingSpeed = 40
local genPerc = 30

function move()
    for i = size, 2, -1 do
        led_array:copyLed(i, i - 1)
        led_array:fadeToBlack(i - 1, fadingSpeed)
    end

    leds.showLeds()
end

function gen()
    if leds.randomBoolean(genPerc) then
        led_array:setHSV(1, led_array.random8(), 255, 255)
    end
end
