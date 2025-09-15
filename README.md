# Steganography
Command-line tool to hide and extract secret messages inside BMP images using Least Significant Bit (LSB) steganography. Supports encoding text into a cover image and decoding it back with integrity checks via a magic signature.

Features
Encode text into 24-bit BMP images using LSB substitution

Decode and recover hidden messages from stego images

Capacity check to ensure message fits in the cover image

Magic signature to verify stego images before decoding

Command-line interface with clear status logs

How it works
Each message bit replaces the least significant bit of image pixel bytes

A magic signature, message length, and payload are embedded in sequence

Visual quality is preserved since LSB changes minimally alter pixels

Project structure
src/: main.c, encode.c, decode.c, bmp_io.c, bit_ops.c

include/: encode.h, decode.h, bmp_io.h, bit_ops.h, common.h

data/: sample BMPs and example messages

build/: binaries and artifacts

tests/: unit tests for BMP I/O and bit packing
