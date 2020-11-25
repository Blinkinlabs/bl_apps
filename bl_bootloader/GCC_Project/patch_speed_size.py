# Patch the bootloader image with a faster SPI speed, and the exact image size. This should improve boot speed

import struct
import math

bl_file = open('output/bl_bootloader.bin','rb')
bl = bytearray(bl_file.read())

header_addr = 0x120
header = bl[header_addr:header_addr+4]
[boot_size, clock_divider, device_id] = struct.unpack('<HBB', header)
print('Read: Device ID:{:02x}, SPI clock divider:{:02x} boot size:{:04x}'.format(device_id, clock_divider, boot_size))

print('Bootloader length:', len(bl))
boot_size = math.ceil(len(bl)/8)

# 1 gives ~6MHz clock, 0 does not work
clock_divider = 1

print('Write: Device ID:{:02x}, SPI clock divider:{:02x} boot size:{:04x}'.format(device_id, clock_divider, boot_size))

header = struct.pack('<HBB', boot_size, clock_divider, device_id)
bl[header_addr:header_addr+4] = header

outfile = open('output/bl_bootloader_patch.bin','wb')
outfile.write(bl)
