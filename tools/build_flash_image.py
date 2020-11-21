import argparse
import struct
import libscrc

# From the bootloader readme:
# 
# |Item                 |Status |Start      |   Size       |    End             |Start    | Size        | End     |
# |-----                |-------|----------:|   ----------:|    -------------:  |--------:| ----------: | -------:|
# |bootloader           |Used   |0x0000_0000|   0x0001_0000|    0x0000_FFFF     |      0  |      65,536 |  65,536 |
# |bootfpga CRC         |Used   |0x0001_0000|             8|    0x0001_0007     | 65,536  |           8 |  65,544 |
# |appfpga CRC          |Future |0x0001_1000|             8|    0x0001_1007     | 69,632  |           8 |  69,640 |
# |appffe CRC           |Future |0x0001_2000|             8|    0x0001_2007     | 73,728  |           8 |  73,736 |
# |M4app CRC            |Used   |0x0001_3000|             8|    0x0001_3007     | 77,824  |           8 |  77,832 |
# |bootloader CRC       |Used   |0x0001_4000|             8|    0x0001_4007     | 81,920  |           8 |  81,928 |
# |bootfpga             |Used   |0x0002_0000|   0x0002_0000|    0x0003_FFFF     | 131,072 |     131,072 | 262,144 |
# |appfpga              |Future |0x0004_0000|   0x0002_0000|    0x0005_FFFF     | 262,144 |     131,072 | 393,216 |
# |appffe               |Future |0x0006_0000|   0x0002_0000|    0x0007_FFFF     | 393,216 |     131,072 | 524,288 |
# |M4app                |Used   |0x0008_0000|   0x0006_E000|    0x000E_DFFF     | 524,288 |     450,560 | 974,848 |

image_size = 0x000EE000  # Size of the whole image

image_layout = {
    "bootloader":{"start":0x00000, "size":0x10000, "crc_start":0x14000},
    "bootfpga"  :{"start":0x20000, "size":0x20000, "crc_start":0x10000},
    "appfpga"   :{"start":0x40000, "size":0x20000, "crc_start":0x11000},
    "appffe"    :{"start":0x60000, "size":0x20000, "crc_start":0x12000},
    "M4app"     :{"start":0x80000, "size":0x6E000, "crc_start":0x13000}
}

parser = argparse.ArgumentParser(description='Build a binary image for QOL-EOS-S3 containing a bootloader and (optional) default application, which can be loaded directly to a SPI flash. This can be used to provision a new board, or to recover a bricked board.')
parser.add_argument("-bootloader", help="Path to bootloader image", type=str,
        default="../bl_bootloader/GCC_Project/output/bin/bl_bootloader.bin")
parser.add_argument("-bootfpga", help="Path to bootfpga image", type=str,
        default="../bl_bootloader/fpga/usb2serial.bin")
parser.add_argument("-M4app", help="Path to application image", type=str)
parser.add_argument("-output", help="Output filename", type=str, default="image.bin")

args_dict = vars(parser.parse_args())

# Create a binary representation of the whole map
image = bytearray([0xff] * image_size)

def add_to_image(image, name, filename, start, size, crc_start):
    # image: firmware image
    # name: display name of section to add 'bootloader'
    # filename: filename to add to section
    # start: starting address to load section
    # size: max size of section
    # crc_start: starting address of 8-byte CRC

    #print(name, filename, start, size, crc_start)

    section_file = open(filename,'rb')
    section = bytearray(section_file.read())

    if len(section) > size:
        print('Section % file size (%) exceeds available partition size (%)'.args(name, len(section), size))
        exit(1)

    # (from the source file) CRC Parameters:
    #  Width  : 32
    #  Poly   : 0x04c11db7
    #  Init   : 0xffffffff
    #  RefIn  : false
    #  RefOut : false
    #  XorOut : 0
    #
    # According to https://reveng.sourceforge.io/crc-catalogue/17plus.htm#crc.cat-bits.32, this matches 'CRC-32/MPEG-2'
    crc = libscrc.mpeg2(section)

    image[start:start+len(section)] = section

    # By inspection of the source code, the 8-byte CRC location contains both a 32-bit CRC and 32-bit image size
    image[crc_start:crc_start+8] = struct.pack('<II', crc, len(section))

    print('added section:{} at:0x{:08x} size:{} crc:0x{:08x}'.format(name, start, len(section), crc))

    return image

# Copy any specified item sections into the image
for item,props in image_layout.items():
    if item in args_dict and args_dict[item] != None:
        image = add_to_image(image, item, args_dict[item], props['start'], props['size'], props['crc_start'])


#print(image)

# Note that the Teensy loader requires an image that is aligned to the sector size (4k for most SPI flashes). Warn
# if our file will need padding
if (len(image) % 0x1000) != 0:
    print("warning: image not aligned to 4k sector size, may need padding before upload.")

outfile = open(args_dict['output'],'wb')
outfile.write(image)

print("Wrote 0x{:x} bytes to {}".format(len(image), args_dict['output']))
