local led_array = leds.new()
local size = led_array:size()

local fadingSpeed = 5

local index1 = 1
local index2 = 1
local index3 = 1
local index4 = 1

local hue1 = 0
local hue2 = 64
local hue3 = 96
local hue4 = 160

local inc1 = 1
local inc2 = 1
local inc3 = 1
local inc4 = 1

function chaser1()
    index1 = index1 + inc1
    if index1 >= size or index1 <= 1 then
        inc1 = inc1 * -1
    end
end

function chaser2()
    index2 = index2 + inc2
    if index2 >= size or index2 <= 1 then
        inc2 = inc2 * -1
    end
end

function chaser3()
    index3 = index3 + inc3
    if index3 >= size or index3 <= 1 then
        inc3 = inc3 * -1
    end
end

function chaser4()
    index4 = index4 + inc4
    if index4 >= size or index4 <= 1 then
        inc4 = inc4 * -1
    end
end

function update()
    led_array:addHue(index1, hue1)
    led_array:addHue(index2, hue2)
    led_array:addHue(index3, hue3)
    led_array:addHue(index4, hue4)

    led_array:fadeAllToBlack(fadingSpeed)

    leds.showLeds()
end