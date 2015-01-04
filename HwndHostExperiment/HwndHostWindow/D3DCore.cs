using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace space
{
	class D3DCore
	{
		[DllImport("D3DCore.dll",CallingConvention=CallingConvention.Cdecl)]
		static extern IntPtr D3DCore_Constructor();

		[DllImport("D3DCore.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void D3DCore_Initialize(IntPtr pNative, IntPtr handle);

		[DllImport("D3DCore.dll",CallingConvention = CallingConvention.Cdecl)]
		static extern void D3DCore_Render(IntPtr pNative);

		private IntPtr _pNative = IntPtr.Zero;

		public D3DCore()
		{
			_pNative = D3DCore_Constructor();
		}

		public void Initialize(IntPtr handle)
		{
			D3DCore_Initialize(_pNative,handle);
		}

		public void Render()
		{
			D3DCore_Render(_pNative);
		}
	}
}
