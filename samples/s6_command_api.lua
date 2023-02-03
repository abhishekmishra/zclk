local zclk = require 'zclk'

--- command handler
local function cmd_handler(cmd)
    print('\t------ lua command ------')
    print('\tname       : ' .. cmd:name())
    print('\tshort name : ' .. cmd:short_name())
    print('\tdescription: ' .. cmd:description()) -- can also use cmd:desc()

    local opt_one = cmd:get_option('option-one')
    print('\toption-one value = [' .. tostring(opt_one:value()) .. ']'
        .. '  type  = [' .. opt_one:type() .. ']')

    local arg_one = cmd:get_argument('arg-one')
    print('\targ-one value = [' .. tostring(arg_one:value()) .. ']'
        .. '  type = [' .. arg_one:type() .. ']')

    return 0
end

-- create a new command
local cmd = zclk.new(arg[0], 'cmd', 'A basic lua command', cmd_handler)
cmd:flag_option("option-one", "o", 0, "dummy test option one")
cmd:string_argument("arg-one", "blah", "dummy string argument one")
cmd:exec(arg)