.pushsection command_data, "ax", %progbits
.incbin "data/Trigger_and_command_pack_withTxt.bin"
.popsection

.pushsection license_data, "ax", %progbits
.incbin "data/CybLicense.bin"
.popsection
