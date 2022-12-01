.pushsection command_data, "ax", %progbits
.incbin "data/Two-stage_pack_withTxt.bin"
.popsection

.pushsection license_data, "ax", %progbits
.incbin "data/CybLicense.bin"
.popsection
