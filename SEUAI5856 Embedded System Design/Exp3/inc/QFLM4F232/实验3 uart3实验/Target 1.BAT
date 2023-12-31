SET PATH=F:\Keil_v5\ARM\ARMCC\Bin;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\Program Files (x86)\Nordic Semiconductor\nrf51\bin\;F:\Cadence\SPB_16.5\OpenAccess\bin\win32\opt;F:\Cadence\SPB_16.5\tools\Capture;F:\Cadence\SPB_16.5\tools\PSpice\Library;F:\Cadence\SPB_16.5\tools\PSpice;F:\Cadence\SPB_16.5\tools\specctra\bin;F:\Cadence\SPB_16.5\tools\fet\bin;F:\Cadence\SPB_16.5\tools\libutil\bin;F:\Cadence\SPB_16.5\tools\bin;F:\Cadence\SPB_16.5\tools\pcb\bin;
SET ARMCC5_ASMOPT=--diag_suppress=9931
SET ARMCC5_CCOPT=--diag_suppress=9931
SET ARMCC5_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=LM4F232H5QD
SET CPU_VENDOR=Texas Instruments
SET UV2_TARGET=Target 1
SET CPU_CLOCK=0x00B71B00
"F:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\rvmdk\startup_rvmdk._ia"
"F:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\rvmdk\lcd.__i"
"F:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\rvmdk\key.__i"
"F:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\rvmdk\pcf8574.__i"
"F:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\rvmdk\main.__i"
"F:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\rvmdk\uart.__i"
"F:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\rvmdk\uartstdio.__i"
"F:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via ".\rvmdk\lcd_hello.lnp"
"F:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" ".\rvmdk\lcd_hello.axf" --i32combined --output ".\rvmdk\lcd_hello.hex"
fromelf --bin --output .\rvmdk\lcd_hello.bin .\rvmdk\lcd_hello.axf
