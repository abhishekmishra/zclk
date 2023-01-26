local zclk = require 'zclk'

local function cmd_handler()
    print('command handler')
end

local cmd = zclk.new(arg[0], 'cmd', 'A basic lua command', cmd_handler)

cmd:exec(arg)