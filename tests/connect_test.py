import time
import cminiopy
import cminiopy.sync as sync

client = sync.connect("0.0.0.0:9000", "adminadmin", "passwordpassword", False)
start = time.time()
for o in sync.list_objects("b", "b/", False):
    print(o)
    break
print(time.time() - start)
