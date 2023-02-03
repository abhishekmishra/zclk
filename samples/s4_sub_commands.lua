local zclk = require 'zclk'

--- command handler
local function cmd_handler(cmd)
    print('lua command handler for ' .. cmd:name())
end

--- sub-command handler
local function sub_cmd_handler(cmd)
    print('lua sub-command handler for ' .. cmd:name())
end

-- create a new command
local cmd = zclk.new(arg[0], 'cmd', 'A basic lua command', cmd_handler)

-- create a new command (which will be the sub-command)
local subcmd1 = zclk.new('subcommand1', 'sub1', 'A basic lua sub-command',
    sub_cmd_handler)
local subcmd2 = zclk.new('subcommand2', 'sub2', 'A basic lua sub-command',
    sub_cmd_handler)

-- add the sub-command to the command
cmd:subcommand(subcmd1)
cmd:subcommand(subcmd2)

-- run the command
cmd:exec(arg)