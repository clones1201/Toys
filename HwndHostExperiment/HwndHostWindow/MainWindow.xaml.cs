using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using space;

namespace HwndHostWindow
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            Loaded += OnUIReady;
        }

        D3DCore renderer = new D3DCore();

        private void OnUIReady(object sender, RoutedEventArgs e)
        {
            MyHwndHost hwndHost = new MyHwndHost();
            hwndHost.Opacity = 0.6;
            HostElement.Child = hwndHost;
            
            renderer.Initialize(hwndHost.HwndHandle);
            CompositionTarget.Rendering += CompositionTarget_Rendering;            
        }

        void CompositionTarget_Rendering(object sender, EventArgs e)
        {
            renderer.Render();
        }
    }
}
