local zclk = require 'zclk'

local function cmd_handler()
    print('command handler')
end

local cmd = zclk.new('BasicCommand', 'cmd', 'A basic lua command', cmd_handler)
print(cmd)
