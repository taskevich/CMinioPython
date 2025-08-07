import cminiopy
import cminiopy.sync as sync

client = sync.connect("localhost:9000", "access", "secret", True)
sync.check(client)
