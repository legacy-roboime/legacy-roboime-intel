using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace TESTEUsb
{

    
    
    class UsbWrapper
    {
        [DllImport("User32.dll", CharSet = CharSet.Auto)]
        private static extern int SystemParametersInfo(
        int lAction, int lParam, int lpvParam, int luWinIni);
       
    }
}
