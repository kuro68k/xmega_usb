# Minimal, portable USB stack

Fork of: https://github.com/kevinmehall/usb

WCID supported added to minimal example. Note that you need at least one endpoint for WCID to work.

Enabled multi-packet mode for IN transfers greater than 64 bytes. These only work when the data buffer is in RAM, data in program memory is copied to a 64 byte buffer before sending. This is mainly to support WCID descriptors.

Added some comments to help configure the stack. Tested and verified operation a little.

Minimal example with WCID and 1 bulk IN/OUT endpoint comes in at around 2.5k flash and 400 bytes of RAM.

MIT license.
