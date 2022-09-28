local led_array = leds.new()
local size = led_array:size()

local width = 5
local hue = 160
local fadingSpeed = 1

function main()
    local startOfGroup = led_array:randomIndex()
    local sat = leds.random8()
    local val = leds.random8(100, 255)
    local endOfGroup = startOfGroup + width
    
    for i = startOfGroup, endOfGroup, 1 do
        if i > size then
            break
        end
        led_array:setHSV(i, hue, sat, val)
    end

    led_array:fadeAllToBlack(fadingSpeed)

    leds.showLeds()
end