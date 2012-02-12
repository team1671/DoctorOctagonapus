ECHO This fanciness is brought to you by Folders!
ECHO You may have to change the text in "Wireless Network Connection"

set varip=10.16.71.30
set varsm=255.255.255.0

REM For the most up-to-date-info, visit http://samanathon.com/set-your-ip-address-via-batch-file/

REM ***** You don’t need to change anything below this line! ******

ECHO Setting IP Address and Subnet Mask
netsh int ip set address name = "Wireless Network Connection" source = static addr = %varip% mask = %varsm%

ECHO Here are the new settings for %computername%:

netsh int ip show config

pause