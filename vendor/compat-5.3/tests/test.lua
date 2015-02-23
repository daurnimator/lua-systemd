#!/usr/bin/env lua

local F, tproxy, writefile, noprint, ___
do
  local type, unpack = type, table.unpack or unpack
  local assert, io = assert, io
  function F(...)
    local args, n = { ... }, select('#', ...)
    for i = 1, n do
      local t = type(args[i])
      if t ~= "string" and t ~= "number" and t ~= "boolean" then
        args[i] = t
      end
    end
    return unpack(args, 1, n)
  end
  function tproxy(t)
    return setmetatable({}, {
      __index = t,
      __newindex = t,
      __len = function() return #t end,
    }), t
  end
  function writefile(name, contents, bin)
    local f = assert(io.open(name, bin and "wb" or "w"))
    f:write(contents)
    f:close()
  end
  function noprint() end
  local sep = ("="):rep(70)
  function ___()
    print(sep)
  end
end

local V = _VERSION:gsub("^.*(%d+)%.(%d+)$", "%1%2")
if jit then V = "jit" end

print( "testing Lua API ..." )
package.path = "../?.lua;"..package.path
package.cpath = "./?-"..V..".so;./?-"..V..".dll;./?.so;./?.dll"
require("compat53")

___''
do
  local t = setmetatable( {}, { __index = { 1, false, "three" } } )
  for i,v in ipairs(t) do
    print("ipairs", i, v)
  end
end


___''
do
  local p, t = tproxy{ "a", "b", "c" }
  print("table.concat", table.concat(p))
  print("table.concat", table.concat(p, ",", 2))
  print("table.concat", table.concat(p, ".", 1, 2))
  print("table.concat", table.concat(t))
  print("table.concat", table.concat(t, ",", 2))
  print("table.concat", table.concat(t, ".", 1, 2))
end


___''
do
  local p, t = tproxy{ "a", "b", "c" }
  table.insert(p, "d")
  print("table.insert", next(p), t[4])
  table.insert(p, 1, "z")
  print("table.insert", next(p),  t[1], t[2])
  table.insert(p, 2, "y")
  print("table.insert", next(p), t[1], t[2], p[3])
  t = { "a", "b", "c" }
  table.insert(t, "d")
  print("table.insert", t[1], t[2], t[3], t[4])
  table.insert(t, 1, "z")
  print("table.insert", t[1], t[2], t[3], t[4], t[5])
  table.insert(t, 2, "y")
  print("table.insert", t[1], t[2], t[3], t[4], t[5])
end


___''
do
  local ps, s = tproxy{ "a", "b", "c", "d" }
  local pd, d = tproxy{ "A", "B", "C", "D" }
  table.move(ps, 1, 4, 1, pd)
  print("table.move", next(pd), d[1], d[2], d[3], d[4])
  pd, d = tproxy{ "A", "B", "C", "D" }
  table.move(ps, 2, 4, 1, pd)
  print("table.move", next(pd), d[1], d[2], d[3], d[4])
  pd, d = tproxy{ "A", "B", "C", "D" }
  table.move(ps, 2, 3, 4, pd)
  print("table.move", next(pd), d[1], d[2], d[3], d[4], d[5])
  table.move(ps, 2, 4, 1)
  print("table.move", next(ps), s[1], s[2], s[3], s[4])
  ps, s = tproxy{ "a", "b", "c", "d" }
  table.move(ps, 2, 3, 4)
  print("table.move", next(ps), s[1], s[2], s[3], s[4], s[5])
  s = { "a", "b", "c", "d" }
  d = { "A", "B", "C", "D" }
  table.move(s, 1, 4, 1, d)
  print("table.move", d[1], d[2], d[3], d[4])
  d = { "A", "B", "C", "D" }
  table.move(s, 2, 4, 1, d)
  print("table.move", d[1], d[2], d[3], d[4])
  d = { "A", "B", "C", "D" }
  table.move(s, 2, 3, 4, d)
  print("table.move", d[1], d[2], d[3], d[4], d[5])
  table.move(s, 2, 4, 1)
  print("table.move", s[1], s[2], s[3], s[4])
  s = { "a", "b", "c", "d" }
  table.move(s, 2, 3, 4)
  print("table.move", s[1], s[2], s[3], s[4], s[5])
end


___''
do
  local p, t = tproxy{ "a", "b", "c", "d", "e" }
  print("table.remove", table.remove(p))
  print("table.remove", next(p), t[1], t[2], t[3], t[4], t[5])
  print("table.remove", table.remove(p, 1))
  print("table.remove", next(p), t[1], t[2], t[3], t[4])
  print("table.remove", table.remove(p, 2))
  print("table.remove", next(p), t[1], t[2], t[3])
  print("table.remove", table.remove(p, 3))
  print("table.remove", next(p), t[1], t[2], t[3])
  p, t = tproxy{}
  print("table.remove", table.remove(p))
  print("table.remove", next(p), next(t))
  t = { "a", "b", "c", "d", "e" }
  print("table.remove", table.remove(t))
  print("table.remove", t[1], t[2], t[3], t[4], t[5])
  print("table.remove", table.remove(t, 1))
  print("table.remove", t[1], t[2], t[3], t[4])
  print("table.remove", table.remove(t, 2))
  print("table.remove", t[1], t[2], t[3])
  print("table.remove", table.remove(t, 3))
  print("table.remove", t[1], t[2], t[3])
  t = {}
  print("table.remove", table.remove(t))
  print("table.remove", next(t))
end

___''
do
  local p, t = tproxy{ 3, 1, 5, 2, 8, 5, 2, 9, 7, 4 }
  table.sort(p)
  print("table.sort", next(p))
  for i,v in ipairs(t) do
    print("table.sort", i, v)
  end
  table.sort(p)
  print("table.sort", next(p))
  for i,v in ipairs(t) do
    print("table.sort", i, v)
  end
  p, t = tproxy{ 9, 8, 7, 6, 5, 4, 3, 2, 1 }
  table.sort(p)
  print("table.sort", next(p))
  for i,v in ipairs(t) do
    print("table.sort", i, v)
  end
  table.sort(p, function(a, b) return a > b end)
  print("table.sort", next(p))
  for i,v in ipairs(t) do
    print("table.sort", i, v)
  end
  p, t = tproxy{ 1, 1, 1, 1, 1 }
  print("table.sort", next(p))
  for i,v in ipairs(t) do
    print("table.sort", i, v)
  end
  t = { 3, 1, 5, 2, 8, 5, 2, 9, 7, 4 }
  table.sort(t)
  for i,v in ipairs(t) do
    print("table.sort", i, v)
  end
  table.sort(t, function(a, b) return a > b end)
  for i,v in ipairs(t) do
    print("table.sort", i, v)
  end
end


___''
do
  local p, t = tproxy{ "a", "b", "c" }
  print("table.unpack", table.unpack(p))
  print("table.unpack", table.unpack(p, 2))
  print("table.unpack", table.unpack(p, 1, 2))
  print("table.unpack", table.unpack(t))
  print("table.unpack", table.unpack(t, 2))
  print("table.unpack", table.unpack(t, 1, 2))
end


___''
print("math.maxinteger", math.maxinteger+1 > math.maxinteger)
print("math.mininteger", math.mininteger-1 < math.mininteger)


___''
print("math.tointeger", math.tointeger(0))
print("math.tointeger", math.tointeger(math.pi))
print("math.tointeger", math.tointeger("hello"))
print("math.tointeger", math.tointeger(math.maxinteger+2.0))
print("math.tointeger", math.tointeger(math.mininteger*2.0))


___''
print("math.type", math.type(0))
print("math.type", math.type(math.pi))
print("math.type", math.type("hello"))


___''
print("math.ult", math.ult(1, 2), math.ult(2, 1))
print("math.ult", math.ult(-1, 2), math.ult(2, -1))
print("math.ult", math.ult(-1, -2), math.ult(-2, -1))
print("math.ult", pcall(math.ult, "x", 2))
print("math.ult", pcall(math.ult, 1, 2.1))
___''


print("testing Lua API for Lua 5.1 ...")

___''
print("debug.getuservalue()", F(debug.getuservalue(false)))
print("debug.setuservalue()", pcall(function()
  debug.setuservalue(false, {})
end))
print("debug.setmetatable()", F(debug.setmetatable({}, {})))


___''
do
   local t = setmetatable({}, {
      __pairs = function() return pairs({ a = "a" }) end,
   })
   for k,v in pairs(t) do
      print("pairs()", k, v)
   end
end


___''
do
   local code = "print('hello world')\n"
   local badcode = "print('blub\n"
   print("load()", pcall(function() load(true) end))
   print("load()", F(load(badcode)))
   print("load()", F(load(code)))
   print("load()", F(load(code, "[L]")))
   print("load()", F(load(code, "[L]", "b")))
   print("load()", F(load(code, "[L]", "t")))
   print("load()", F(load(code, "[L]", "bt")))
   local f = load(code, "[L]", "bt", {})
   print("load()", pcall(f))
   f = load(code, "[L]", "bt", { print = noprint })
   print("load()", pcall(f))
   local bytecode = string.dump(f)
   print("load()", F(load(bytecode)))
   print("load()", F(load(bytecode, "[L]")))
   print("load()", F(load(bytecode, "[L]", "b")))
   print("load()", F(load(bytecode, "[L]", "t")))
   print("load()", F(load(bytecode, "[L]", "bt")))
   f = load(bytecode, "[L]", "bt", {})
   print("load()", pcall(f))
   f = load(bytecode, "[L]", "bt", { print = noprint })
   print("load()", pcall(f))
   local function make_loader(code)
      local mid = math.floor( #code/2 )
      local array = { code:sub(1, mid), code:sub(mid+1) }
      local i = 0
      return function()
         i = i + 1
         return array[i]
      end
   end
   print("load()", F(load(make_loader(badcode))))
   print("load()", F(load(make_loader(code))))
   print("load()", F(load(make_loader(code), "[L]")))
   print("load()", F(load(make_loader(code), "[L]", "b")))
   print("load()", F(load(make_loader(code), "[L]", "t")))
   print("load()", F(load(make_loader(code), "[L]", "bt")))
   f = load(make_loader(code), "[L]", "bt", {})
   print("load()", pcall(f))
   f = load(make_loader(code), "[L]", "bt", { print = noprint })
   print("load()", pcall(f))
   print("load()", F(load(make_loader(bytecode))))
   print("load()", F(load(make_loader(bytecode), "[L]")))
   print("load()", F(load(make_loader(bytecode), "[L]", "b")))
   print("load()", F(load(make_loader(bytecode), "[L]", "t")))
   print("load()", F(load(make_loader(bytecode), "[L]", "bt")))
   f = load(make_loader(bytecode), "[L]", "bt", {})
   print("load()", pcall(f))
   f = load(make_loader(bytecode), "[L]", "bt", { print = noprint })
   print("load()", pcall(f))
   writefile("good.lua", code)
   writefile("bad.lua", badcode)
   writefile("good.luac", bytecode, true)
   print("loadfile()", F(loadfile("bad.lua")))
   print("loadfile()", F(loadfile("good.lua")))
   print("loadfile()", F(loadfile("good.lua", "b")))
   print("loadfile()", F(loadfile("good.lua", "t")))
   print("loadfile()", F(loadfile("good.lua", "bt")))
   f = loadfile("good.lua", "bt", {})
   print("loadfile()", pcall(f))
   f = loadfile("good.lua", "bt", { print = noprint })
   print("loadfile()", pcall(f))
   print("loadfile()", F(loadfile("good.luac")))
   print("loadfile()", F(loadfile("good.luac", "b")))
   print("loadfile()", F(loadfile("good.luac", "t")))
   print("loadfile()", F(loadfile("good.luac", "bt")))
   f = loadfile("good.luac", "bt", {})
   print("loadfile()", pcall(f))
   f = loadfile("good.luac", "bt", { print = noprint })
   print("loadfile()", pcall(f))
   os.remove("good.lua")
   os.remove("bad.lua")
   os.remove("good.luac")
end


___''
do
   local function func(throw)
      if throw then
         error("argh")
      else
         return 1, 2, 3
      end
   end
   local function tb(err) return "|"..err.."|" end
   print("xpcall()", xpcall(func, debug.traceback, false))
   print("xpcall()", xpcall(func, debug.traceback, true))
   print("xpcall()", xpcall(func, tb, true))
   local function func2(cb)
     print("xpcall()", xpcall(cb, debug.traceback, "str"))
   end
   local function func3(cb)
     print("pcall()", pcall(cb, "str"))
   end
   local function cb(arg)
      coroutine.yield(2)
      return arg
   end
   local c = coroutine.wrap(func2)
   print("xpcall()", c(cb))
   print("xpcall()", c())
   local c = coroutine.wrap(func3)
   print("pcall()", c(cb))
   print("pcall()", c())
end


___''
do
   local t = setmetatable({ 1 }, { __len = function() return 5 end })
   print("rawlen()", rawlen(t), rawlen("123"))
end


___''
print("os.execute()", os.execute("exit 1"))
io.flush()
print("os.execute()", os.execute("echo 'hello world!'"))
io.flush()
print("os.execute()", os.execute("no_such_file"))


___''
do
   local t = table.pack("a", nil, "b", nil)
   print("table.(un)pack()", t.n, table.unpack(t, 1, t.n))
end


___''
do
   print("coroutine.running()", F(coroutine.wrap(function()
      return coroutine.running()
   end)()))
   print("coroutine.running()", F(coroutine.running()))
   local main_co, co1, co2 = coroutine.running()
   -- coroutine.yield
   print("coroutine.yield()", pcall(function()
      coroutine.yield(1, 2, 3)
   end))
   print("coroutine.yield()", coroutine.wrap(function()
      coroutine.yield(1, 2, 3)
   end)())
   print("coroutine.resume()", coroutine.resume(main_co, 1, 2, 3))
   co1 = coroutine.create(function(a, b, c)
      print("coroutine.resume()", a, b, c)
      return a, b, c
   end)
   print("coroutine.resume()", coroutine.resume(co1, 1, 2, 3))
   co1 = coroutine.create(function()
      print("coroutine.status()", "[co1] main is", coroutine.status(main_co))
      print("coroutine.status()", "[co1] co2 is", coroutine.status(co2))
   end)
   co2 = coroutine.create(function()
      print("coroutine.status()", "[co2] main is", coroutine.status(main_co))
      print("coroutine.status()", "[co2] co2 is", coroutine.status(co2))
      coroutine.yield()
      coroutine.resume(co1)
   end)
   print("coroutine.status()", coroutine.status(main_co))
   print("coroutine.status()", coroutine.status(co2))
   coroutine.resume(co2)
   print("coroutine.status()", F(coroutine.status(co2)))
   coroutine.resume(co2)
   print("coroutine.status()", F(coroutine.status(co2)))
end


___''
print("math.log()", math.log(1000))
print("math.log()", math.log(1000, 10))


___''
do
   local path, prefix = "./?.lua;?/init.lua;../?.lua", "package.searchpath()"
   print(prefix, package.searchpath("no.such.module", path))
   print(prefix, package.searchpath("no.such.module", ""))
   print(prefix, package.searchpath("compat52", path))
   print(prefix, package.searchpath("no:such:module", path, ":", "|"))
end


___''
do
   local function mod_func() return {} end
   local function my_searcher(name)
      if name == "my.module" then
         print("package.searchers", "my.module found")
         return mod_func
      end
   end
   local function my_searcher2(name)
      if name == "my.module" then
         print("package.searchers", "my.module found 2")
         return mod_func
      end
   end
   table.insert(package.searchers, my_searcher)
   require("my.module")
   package.loaded["my.module"] = nil
   local new_s = { my_searcher2 }
   for i,f in ipairs(package.searchers) do
      new_s[i+1] = f
   end
   package.searchers = new_s
   require("my.module")
end


___''
do
   print("string.find()", ("abc\0abc\0abc"):find("[^a\0]+"))
   print("string.find()", ("abc\0abc\0abc"):find("%w+\0", 5))
   for x in ("abc\0def\0ghi"):gmatch("[^\0]+") do
      print("string.gmatch()", x)
   end
   for x in ("abc\0def\0ghi"):gmatch("%w*\0") do
      print("string.gmatch()", #x)
   end
   print("string.gsub()", ("abc\0def\0ghi"):gsub("[\0]", "X"))
   print("string.gsub()", ("abc\0def\0ghi"):gsub("%w*\0", "X"))
   print("string.gsub()", ("abc\0def\0ghi"):gsub("%A", "X"))
   print("string.match()", ("abc\0abc\0abc"):match("([^\0a]+)"))
   print("string.match()", #("abc\0abc\0abc"):match(".*\0"))
   print("string.rep()", string.rep("a", 0))
   print("string.rep()", string.rep("b", 1))
   print("string.rep()", string.rep("c", 4))
   print("string.rep()", string.rep("a", 0, "|"))
   print("string.rep()", string.rep("b", 1, "|"))
   print("string.rep()", string.rep("c", 4, "|"))
   local _tostring = tostring
   function tostring(v)
      if type(v) == "number" then
         return "(".._tostring(v)..")"
      else
         return _tostring(v)
      end
   end
   print("string.format()", string.format("%q", "\"\\\0000\0010\r0\n0\t0\""))
   print("string.format()", string.format("%12.3fx%%sxx%.6s", 3.1, {}))
   print("string.format()", string.format("%-3f %%%s %%s", 3.1, true))
   print("string.format()", string.format("% 3.2g %%d %%%s", 3.1, nil))
   print("string.format()", string.format("%+3d %%d %%%%%10.6s", 3, io.stdout))
   print("string.format()", pcall(function()
      print("string.format()", string.format("%d %%s", {}))
   end))
   tostring = _tostring
end


___''
do
   print("io.write()", io.type(io.write("hello world\n")))
   local f = assert(io.tmpfile())
   print("file:write()", io.type(f:write("hello world\n")))
   f:close()
end


___''
do
   writefile("data.txt", "123 18.8 hello world\ni'm here\n")
   for a,b in io.lines("test.lua", 2, "*l") do
      print("io.lines()", a, b)
      break
   end
   for l in io.lines("test.lua") do
      print("io.lines()", l)
      break
   end
   for n1,n2,rest in io.lines("data.txt", "*n", "*n", "*a") do
      print("io.lines()", n1, n2, rest)
   end
   for l in io.lines("data.txt") do
      print("io.lines()", l)
   end
   print("io.lines()", pcall(function()
      for l in io.lines("data.txt", "*x") do print(l) end
   end))
   print("io.lines()", pcall(function()
      for l in io.lines("no_such_file.txt") do print(l) end
   end))
   local f = assert(io.open("test.lua", "r"))
   for a,b in f:lines(2, "*l") do
      print("file:lines()", a, b)
      break
   end
   f:close()
   f = assert(io.open("data.txt", "r"))
   for n1,n2,rest in f:lines("*n", "*n", "*a") do
      print("file:lines()", n1, n2, rest)
   end
   f:close()
   f = assert(io.open("data.txt", "r"))
   for l in f:lines() do
      print("file:lines()", l)
   end
   f:close()
   print("file:lines()", pcall(function()
      for l in f:lines() do print(l) end
   end))
   print("file:lines()", pcall(function()
      local f = assert(io.open("data.txt", "r"))
      for l in f:lines("*l", "*x") do print(l) end
      f:close()
   end))
   os.remove("data.txt")
end
___''


print("testing C API ...")
local mod = require("testmod")
___''
print(mod.isinteger(1))
print(mod.isinteger(0))
print(mod.isinteger(1234567))
print(mod.isinteger(12.3))
print(mod.isinteger(math.huge))
print(mod.isinteger(math.sqrt(-1)))


___''
print(mod.rotate(1, 1, 2, 3, 4, 5, 6))
print(mod.rotate(-1, 1, 2, 3, 4, 5, 6))
print(mod.rotate(4, 1, 2, 3, 4, 5, 6))
print(mod.rotate(-4, 1, 2, 3, 4, 5, 6))


___''
print(mod.strtonum("+123"))
print(mod.strtonum(" 123 "))
print(mod.strtonum("-1.23"))
print(mod.strtonum(" 123 abc"))
print(mod.strtonum("jkl"))


___''
local a, b, c = mod.requiref()
print( type(a), type(b), type(c),
       a.boolean, b.boolean, c.boolean,
       type(requiref1), type(requiref2), type(requiref3))

___''
local proxy, backend = {}, {}
setmetatable(proxy, { __index = backend, __newindex = backend })
print(rawget(proxy, 1), rawget(backend, 1))
print(mod.getseti(proxy, 1))
print(rawget(proxy, 1), rawget(backend, 1))
print(mod.getseti(proxy, 1))
print(rawget(proxy, 1), rawget(backend, 1))

-- tests for Lua 5.1
___''
print(mod.tonumber(12))
print(mod.tonumber("12"))
print(mod.tonumber("0"))
print(mod.tonumber(false))
print(mod.tonumber("error"))

___''
print(mod.tointeger(12))
print(mod.tointeger("12"))
print(mod.tointeger("0"))
print( "aaa" )
print(mod.tointeger(math.pi))
print( "bbb" )
print(mod.tointeger(false))
print(mod.tointeger("error"))

___''
print(mod.len("123"))
print(mod.len({ 1, 2, 3}))
print(pcall(mod.len, true))
local ud, meta = mod.newproxy()
meta.__len = function() return 5 end
print(mod.len(ud))
meta.__len = function() return true end
print(pcall(mod.len, ud))

___''
print(mod.copy(true, "string", {}, 1))

___''
print(mod.rawxetp())
print(mod.rawxetp("I'm back"))

___''
print(F(mod.globals()), mod.globals() == _G)

___''
local t = {}
print(F(mod.subtable(t)))
local x, msg = mod.subtable(t)
print(F(x, msg, x == t.xxx))

___''
print(F(mod.udata()))
print(mod.udata("nosuchtype"))

___''
print(F(mod.uservalue()))

___''
print(mod.getupvalues())

___''
print(mod.absindex("hi", true))

___''
print(mod.arith(2, 1))
print(mod.arith(3, 5))

___''
print(mod.compare(1, 1))
print(mod.compare(2, 1))
print(mod.compare(1, 2))

___''
print(mod.tolstring("string"))
local t = setmetatable({}, {
  __tostring = function(v) return "mytable" end
})
print(mod.tolstring(t))
local t = setmetatable({}, {
  __tostring = function(v) return nil end
})
print(pcall(mod.tolstring, t))

___''
print(mod.buffer())
___''

