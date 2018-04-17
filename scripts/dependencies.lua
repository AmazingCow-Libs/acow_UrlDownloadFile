#!/usr/env lua

--------------------------------------------------------------------------------
-- Imports                                                                    --
--------------------------------------------------------------------------------
local PATH = require("PATH");
local lfs  = require("lfs" );

--------------------------------------------------------------------------------
-- Functions                                                                  --
--------------------------------------------------------------------------------
function get_script_fullpath()
    local str = debug.getinfo(2, "S").source:sub(2)
    return str;
 end

function print_dependencies(deps)
    for i, dep in ipairs(deps) do
        print(dep);
    end
end

function download_dependencies(target_dir, deps)
    os.execute(string.format(
        "github-clone.lua %s %s",
        target_dir,
        table.concat(deps, " ")
    ));
end

function show_usage(error_arg)
    print(string.format("Invalid arg: %s", error_arg));
    print("Usage:\n\tdependencies.lua [download] [print]");
end

--------------------------------------------------------------------------------
-- Variables                                                                  --
--------------------------------------------------------------------------------
local script_fullpath = get_script_fullpath();
local script_dir      = PATH.dirname(script_fullpath);
local target_dir      = PATH.join(script_dir, "../libs");

local dependencies = {
    "AmazingCow-Libs/acow_c_goodies",
    "AmazingCow-Libs/acow_cpp_goodies",
    "AmazingCow-Libs/acow_IO"
};


--------------------------------------------------------------------------------
-- Script                                                                     --
--------------------------------------------------------------------------------
args = {};
if(#arg == 0) then
    table.insert(args, "print");
else
    args = arg;
end

for i, curr_arg in ipairs(args) do
    if(curr_arg == "print") then
        print_dependencies(dependencies);
    elseif curr_arg == "download" then
        download_dependencies(target_dir, dependencies);
    else
        show_usage(curr_arg);
    end
end
