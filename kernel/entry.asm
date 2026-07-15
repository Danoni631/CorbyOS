; CODED ORIGINALLY BY Mell-o-tron, in 08/27/2021
[bits 32]
[global entry]

entry:
    [extern main]
    call main
    jmp $

[global bootscr]
[global wallpaper]
[global doom]
[global usericon]
[global nonex]
[global txt]
[global run]
[global bootup]

[global program1]
[global program2]

bootsrc: incbin "CorbyOS/kernel/include/images/logo.bmp"
wallpaper: incbin "CorbyOS/kernel/include/images/wallpaper.bmp"
doom: incbin "CorbyOS/kernel/include/doomgeneric"
usericon: incbin "CorbyOS/kernel/include/images/usericon.bmp"
nonex: incbin "CorbyOS/kernel/include/images/filegeneric.bmp"
txt: incbin "CorbyOS/kernel/include/images/textfile.bmp"
run: incbin "CorbyOS/kernel/include/images/program.bmp"
bootup: incbin "CorbyOS/kernel/include/images/bootup.pcm"
program1: incbin "CorbyOS/kernel/programs/binaries/program1.elf"
program2: incbin "CorbyOS/kernel/programs/binaries/program2.elf"