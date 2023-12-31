SET PATH=C:\Keil\ARM\ARMCC\bin\;C:\ProgramData\Oracle\Java\javapath;C:\Python35\Scripts\;C:\Python35\;C:\Program Files (x86)\Intel\iCLS Client\;C:\Program Files\Intel\iCLS Client\;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\Program Files\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files\Intel\Intel(R) Management Engine Components\IPT;C:\Program Files (x86)\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files (x86)\Intel\Intel(R) Management Engine Components\IPT;C:\Program Files (x86)\ATI Technologies\ATI.ACE\Core-Static;C:\Program Files\Intel\WiFi\bin\;C:\Program Files\Common Files\Intel\WirelessCommon\;C:\Program Files (x86)\Common Files\lenovo\easyplussdk\bin;C:\SWTOOLS\ReadyApps;C:\Program Files (x86)\Common Files\Lenovo;C:\Program Files\MATLAB\R2015a\runtime\win64;C:\Program Files\MATLAB\R2015a\bin;C:\Program Files (x86)\Microsoft Visual Studio\Common\Tools\WinNT;C:\Program Files (x86)\Microsoft Visual Studio\Common\MSDev98\Bin;C:\Program Files (x86)\Microsoft Visual Studio\Common\Tools;C:\Program Files (x86)\Microsoft Visual Studio\VC98\bin;C:\Users\Think\AppData\Local\Microsoft\WindowsApps;C:\Python35;
SET CPU_TYPE=LM4F232H5QD
SET CPU_VENDOR=Texas Instruments
SET UV2_TARGET=Target 1
SET CPU_CLOCK=0x00B71B00
"C:\Keil\ARM\ARMCC\bin\ArmAsm" --Via ".\rvmdk\startup_rvmdk._ia"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\rvmdk\main.__i"
"C:\Keil\ARM\ARMCC\bin\ArmLink" --Via ".\rvmdk\lcd_hello.lnp"
"C:\Keil\ARM\ARMCC\bin\fromelf.exe" ".\rvmdk\lcd_hello.axf" --i32combined --output ".\rvmdk\lcd_hello.hex"
fromelf --bin --output .\rvmdk\lcd_hello.bin .\rvmdk\lcd_hello.axf
