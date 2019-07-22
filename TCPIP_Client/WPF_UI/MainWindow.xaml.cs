using System;
using System.Runtime.InteropServices;
using System.Windows;

namespace WPF_UI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public static class Native
        {
            public delegate void MessageRecievedCallbackDelegate(IntPtr ptr);

            [DllImport("TCPIP_CLIENT_DLL.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
            public static extern void ClientMainLoop();

            [DllImport("TCPIP_CLIENT_DLL.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
            public static extern void setCallbackMessageReceived([MarshalAs(UnmanagedType.FunctionPtr)] MessageRecievedCallbackDelegate ptr);

            [DllImport("TCPIP_CLIENT_DLL.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
            public static extern void ClientSendMessage([MarshalAs(UnmanagedType.AnsiBStr)] string szStr);

            [DllImport("TCPIP_CLIENT_DLL.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
            public static extern void ClientTerminate();
        }

        public MainWindow()
        {
            InitializeComponent();
        }
        
        private void button_SendMesage_OnClick(object sender, RoutedEventArgs e)
        {
            ////TODO how to check which side used
            //OutputTextBox.TextAlignment = TextAlignment.Left;

            OutputTextBox_PrintText(InputTextBox.Text);

            //TODO send message to selected Client
            Native.ClientSendMessage(InputTextBox.Text);

            InputTextBox.Clear();
            InputTextBox.Text = "...enter your message";
        }

        public void MessageRecievedCallback(IntPtr ptr)
        {
            string text = Marshal.PtrToStringAnsi(ptr);
            OutputTextBox_PrintText(text);
        }

        public void OutputTextBox_PrintText(string text)
        {          
            //TODO how to check which side used
            OutputTextBox.Text = OutputTextBox.Text + "\n" + text;
        }
        public void ClientsListBox_Initialize(string text)
        {
            //TODO not implemented
        }

        public void ClientsListBox_Add(string text)
        {
            //TODO not implemented
        }
        public void ClientsListBox_Remove(string text)
        {
            //TODO not implemented
        }

        //private string prepareText(string text, ETextSide side)
        //{
        //    if (side == ETextSide.eLeft)
        //    {
        //        //TODO 
        //        while (text.Length < OutputTextBox.ActualWidth)
        //        {

        //        }
        //    }
        //}
        private void InputTextBox_IsKeyboardFocusWithinChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            InputTextBox.Clear();
        }

        private void Button_Exit_OnClick(object sender, RoutedEventArgs e)
        {
            Native.ClientTerminate();
            Application.Current.Shutdown();
        }
    }
}
