SET PATH=D:\worksoftware\Keil\ARM\ARMCC\bin\;D:\worksoftware\CTEX\CTeX\ty\bin;D:\worksoftware\CTEX\Ghostscript\gs9.00\bin;D:\worksoftware\CTEX\GSview\gsview;D:\worksoftware\CTEX\WinEdt;C:\Program Files (x86)\Common Files\ADLINK\DAQPilot\;C:\Program Files\Common Files\ADLINK\DAQPilot;C:\Program Files\Common Files\ADLINK\DAQPilot\Microsoft.VC80.MFC;C:\Program Files\Common Files\ADLINK\DAQPilot\Microsoft.VC80.CRT;C:\Program Files (x86)\Common Files\ADLINK\DAQPilot\Microsoft.VC80.CRT;C:\Program Files (x86)\Common Files\ADLINK\DAQPilot\Microsoft.VC80.MFC;C:\Program Files (x86)\IVI Foundation\VISA\WinNT\Bin\;C:\Program Files (x86)\IVI Foundation\VISA\WinNT\Bin;C:\Program Files\IVI Foundation\VISA\Win64\Bin\;C:\Program Files (x86)\Windows Kits\10\Windows Performance Toolkit\;C:\Program Files\Microsoft SQL Server\130\Tools\Binn\;C:\Program Files (x86)\IDM Computer Solutions\UltraEdit\;C:\Program Files (x86)\IDM Computer Solutions\UltraCompare\
SET CPU_TYPE=LM4F232H5QD
SET CPU_VENDOR=Texas Instruments
SET UV2_TARGET=Target 1
SET CPU_CLOCK=0x00B71B00
"D:\worksoftware\Keil\ARM\ARMCC\bin\ArmAsm" --Via ".\rvmdk\startup_rvmdk._ia"
"D:\worksoftware\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\rvmdk\lcd.__i"
"D:\worksoftware\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\rvmdk\key.__i"
"D:\worksoftware\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\rvmdk\pcf8574.__i"
"D:\worksoftware\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\rvmdk\main.__i"
"D:\worksoftware\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\rvmdk\uart.__i"
"D:\worksoftware\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\rvmdk\uartstdio.__i"
"D:\worksoftware\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\rvmdk\ustdlib.__i"
"D:\worksoftware\Keil\ARM\ARMCC\bin\ArmLink" --Via ".\rvmdk\lcd_hello.lnp"
"D:\worksoftware\Keil\ARM\ARMCC\bin\fromelf.exe" ".\rvmdk\lcd_hello.axf" --i32combined --output ".\rvmdk\lcd_hello.hex"
fromelf --bin --output .\rvmdk\lcd_hello.bin .\rvmdk\lcd_hello.axf
