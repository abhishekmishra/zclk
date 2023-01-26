local zclk = require 'zclk'

local function cmd_handler(cmd)
    print('lua command handler for ' .. tostring(cmd))

    return 0
end

local cmd = zclk.new(arg[0], 'cmd', 'A basic lua command', cmd_handler)

cmd:exec(arg)