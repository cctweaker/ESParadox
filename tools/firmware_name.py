Import("env")

my_flags = env.ParseFlags(env['BUILD_FLAGS'])
defines = {k: v for (k, v) in my_flags.get("CPPDEFINES")}
print(defines)

env.Replace(PROGNAME="%s_v%s" % (defines.get("FWN") or "ESParadox", defines.get("VERSION") or "0.01a"))