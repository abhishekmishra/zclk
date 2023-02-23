local zclk = require 'zclk'

local function cmd_handler(cmd)
    print('lua command handler for ' .. tostring(cmd))

    local opt_one = cmd:get_option('option-one')
    print(opt_one)
    print('opt value = ' .. tostring(opt_one:value()))
    print('opt type  = ' .. opt_one:type())

    local arg_one = cmd:get_argument('arg-one')
    print(arg_one)
    print('arg value = ' .. tostring(arg_one:value()))
    print('arg type  = ' .. arg_one:type())
    return 0
end

local cmd = zclk.new(arg[0], 'cmd', 'A basic lua command', cmd_handler)
cmd:flag_option("option-one", "o", "dummy test option one")
cmd:string_argument("arg-one", "blah", "dummy string argument one")
cmd:exec(arg)
