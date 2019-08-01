using System;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows;
using System.Windows.Input;

namespace WPF_UI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Thread _threadDllMain;

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

            [DllImport("TCPIP_CLIENT_DLL.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
            public static extern void SetConnectionParams([MarshalAs(UnmanagedType.AnsiBStr)] string name, [MarshalAs(UnmanagedType.AnsiBStr)] string ip, [MarshalAs(UnmanagedType.AnsiBStr)] string port);
        }

        public MainWindow()
        {
            InitializeComponent();

            Native.setCallbackMessageReceived(MessageRecievedCallback);

            
            Native.SetConnectionParams("WPF_UI", "127.0.0.1", "7700");

            _threadDllMain = new Thread(Native.ClientMainLoop);
            _threadDllMain.Start();
        }
        
        private void button_SendMesage_OnClick(object sender, RoutedEventArgs e)
        {
            ////TODO how to check which side used
            /// 
            string text = InputTextBox.Text;
            if (!string.IsNullOrEmpty(text) && text != "...enter your message")
                OutputTextBox_PrintText(InputTextBox.Text);

            //TODO send message to selected Client
            Native.ClientSendMessage(InputTextBox.Text);

            InputTextBox.Clear();
        }

        public void MessageRecievedCallback(IntPtr ptr)
        {
            string text = Marshal.PtrToStringAnsi(ptr);
            OutputTextBox_PrintText(text);
        }

        public void OutputTextBox_PrintText(string text)
        {
            OutputTextBox.Text = OutputTextBox.Text + "\n" + text;
            //TODO how to check which side used
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

        private void Button_Exit_OnClick(object sender, RoutedEventArgs e)
        {
            Native.ClientTerminate();

            _threadDllMain.Abort();
            while (_threadDllMain.ThreadState != ThreadState.Stopped);

            Application.Current.Shutdown();
        }

        //private void InputTextBox_OnIsKeyboardFocusedChanged(object sender, DependencyPropertyChangedEventArgs e)
        //{
        //    InputTextBox.Clear();
        //}
        private void InputTextBox_OnGotKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e)
        {
                InputTextBox.Clear();
        }

        private void InputTextBox_OnLostKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e)
        {
            InputTextBox.Text = "...enter your message";
        }

        private void InputNameBox_OnGotKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e)
        {
            inputNameBox.Clear();
        }

        private void NameOk_OnClick(object sender, RoutedEventArgs e)
        {
        }
    }
}
