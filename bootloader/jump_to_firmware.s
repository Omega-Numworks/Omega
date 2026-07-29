
.syntax unified
.section .text.jump_to_firmware
.align 2
.thumb
.type jump_to_firmware, %function
.global jump_to_firmware
jump_to_firmware:
  msr msp, r0
  bx r1
