.pushsection command_data, "ax", %progbits
.incbin "data/One-stage_pack_withTxt.bin"
.popsection

.pushsection license_data, "ax", %progbits
.incbin "data/CybLicense.bin"
.popsection
