import gzip
import shutil
import os
Import("env","projenv")

def copycompress(source, target, env):
    SOURCE_FILE = env.subst("$BUILD_DIR") + os.sep + env.subst("$PROGNAME") + ".bin"
    GZIP_FILE = env.subst("release") + os.sep + env.subst("$PROGNAME") + ".gz"
    BIN_FILE = env.subst("release") + os.sep + env.subst("$PROGNAME") + ".bin"
    shutil.copy(SOURCE_FILE, BIN_FILE)

    with open(SOURCE_FILE, 'rb') as f_in:
        with gzip.open(GZIP_FILE, 'wb') as f_out:
            shutil.copyfileobj(f_in, f_out)

    ORG_FIRMWARE_SIZE = os.stat(SOURCE_FILE).st_size
    GZ_FIRMWARE_SIZE = os.stat(GZIP_FILE).st_size

    print("Compression reduced firmware size by {:.0f}% (was {} bytes, now {} bytes)".format((GZ_FIRMWARE_SIZE / ORG_FIRMWARE_SIZE) * 100, ORG_FIRMWARE_SIZE, GZ_FIRMWARE_SIZE))

env.AddPostAction("buildprog",copycompress)