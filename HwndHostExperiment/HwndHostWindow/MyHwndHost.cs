using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Interop;

namespace HwndHostWindow
{
    internal class MyHwndHost : HwndHost
    {
        IntPtr hwndHost = IntPtr.Zero;
        public IntPtr HwndHandle
        {
            get { return hwndHost; }
        }
        int hostWidth = 200, hostHeight = 200;
        override protected HandleRef BuildWindowCore(HandleRef parent)
        {
            hwndHost = CreateWindowEx(0, "static", "",
                           WS_CHILD | WS_VISIBLE,
                           0, 0,
                           hostWidth, hostHeight,
                           parent.Handle,
                           (IntPtr)HOST_ID,
                           IntPtr.Zero,
                           0);

            return new HandleRef(null, hwndHost);
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            DestroyWindow(hwndHost);
        }
        internal const int
            WS_CHILD = 0x40000000,
            WS_VISIBLE = 0x10000000,
            LBS_NOTIFY = 0x00000001,
            HOST_ID = 0x00000002,
            LISTBOX_ID = 0x00000001,
            WS_VSCROLL = 0x00200000,
            WS_BORDER = 0x00800000;

        [DllImport("user32.dll", EntryPoint = "CreateWindowEx", CharSet = CharSet.Unicode)]
        internal static extern IntPtr CreateWindowEx(int dwExStyle,
                                                      string lpszClassName,
                                                      string lpszWindowName,
                                                      int style,
                                                      int x, int y,
                                                      int width, int height,
                                                      IntPtr hwndParent,
                                                      IntPtr hMenu,
                                                      IntPtr hInst,
                                                      [MarshalAs(UnmanagedType.AsAny)] object pvParam);
        [DllImport("user32.dll", EntryPoint = "DestroyWindow", CharSet = CharSet.Unicode)]
        internal static extern bool DestroyWindow(IntPtr hwnd);
    }
}
