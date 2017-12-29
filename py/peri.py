from bluepy import btle

print('Connecting...')

dev = btle.Peripheral("b8:27:eb:c3:e4:99")

print("Services...")

for svc in dev.getServices():
    print("service:", svc)
for cha in dev.getCharacteristics():
    print("char:", cha)

ha = "ha"
ha_byte = str.encode(ha)

dev.getCharacteristics()[-1].write(ha_byte, withResponse=True)

print("Writing successfully!")
