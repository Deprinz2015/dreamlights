local led_array = leds.new()
local size = led_array:size()

function main()
    led_array:setHSV(1, 160, led_array.random8(), led_array.random8(100, 255))

    for i = size, 2, -1 do
        led_array:copyLed(i, i - 1)
    end

    leds.showLeds()
end