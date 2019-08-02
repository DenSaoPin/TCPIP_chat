using System;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows;
using System.Windows.Input;
using AttachedPropertyTest;

namespace WPF_UI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Thread _threadDllMain;
        private bool _clientExecuted = false;

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
            public static extern bool ClientIsStarted();

            [DllImport("TCPIP_CLIENT_DLL.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
            public static extern void SetConnectionParams([MarshalAs(UnmanagedType.AnsiBStr)] string name, [MarshalAs(UnmanagedType.AnsiBStr)] string ip, [MarshalAs(UnmanagedType.AnsiBStr)] string port);
        }
        public static class ClientInfo
        {
            public static string Name;
            public static string Adress;
            public static string Port;
        }
        public MainWindow()
        {
            InitializeComponent();

            Hide();

            UserInfoWindow userInfoWindow = new UserInfoWindow();
            userInfoWindow.Show();

            TextBoxUtilities.SetAlwaysScrollToEnd(OutputTextBox, true);
        }

        public void MainLoop()
        {
            Native.SetConnectionParams(ClientInfo.Name, ClientInfo.Adress, ClientInfo.Port);
            Native.setCallbackMessageReceived(MessageRecievedCallback);

            //if (Native.ClientIsStarted())
            //{
                _threadDllMain = new Thread(ClientThreadFunc);
                _threadDllMain.Start();

                _clientExecuted = true;
            //}
        }

        private static void  ClientThreadFunc()
        {
            try
            {
                Native.ClientMainLoop();
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            //    throw;
            }
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
        public void Button_Exit_OnClick(object sender, RoutedEventArgs e)
        {
            if (_clientExecuted)
            {
                Native.ClientTerminate();

                _threadDllMain.Abort();
                while (_threadDllMain.ThreadState != ThreadState.Stopped || _threadDllMain.ThreadState != ThreadState.Unstarted) ;
            }
            Application.Current.Shutdown();
        }

        private void InputTextBox_OnGotKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e)
        {
            InputTextBox.Clear();
        }

        //private void InputTextBox_OnLostKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e)
        //{
        //    InputTextBox.Text = "...enter your message";
        //}

        //private void InputTextBox_TextChanged(object sender, System.Windows.Controls.TextChangedEventArgs e)
        //{
        //    if (IsLoaded && string.IsNullOrEmpty(InputTextBox.Text))
        //        InputTextBox.Text = "...enter your message";
        //}
    }
}
