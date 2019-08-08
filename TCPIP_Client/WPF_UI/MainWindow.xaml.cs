using System;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows;
using System.Windows.Input;
using AttachedPropertyTest;

namespace WPF_UI
{
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

            _threadDllMain = new Thread(ClientThreadFunc);
            _threadDllMain.Start();

            _clientExecuted = true;
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
                ShowText(text, ETextAligment.eLeft);
            //TODO send message to selected Client
            Native.ClientSendMessage(InputTextBox.Text);

            InputTextBox.Clear();
        }
        public enum ETextAligment
        {
            eUnknown,
            eLeft,
            eRight,
        }

        public void ShowText(string text, ETextAligment aligment)
        {
            text += "\n";
            switch (aligment)
            {
                case ETextAligment.eLeft:


                    break;
                case ETextAligment.eRight:
                    if (OutputTextBox.ActualWidth > text.Length)
                    {
                        double tail = OutputTextBox.ActualWidth % (double)(text.Length);
                    }
                    else
                    {
                        while (text.Length < OutputTextBox.MaxLength)
                        {
                            text = " " + text;
                        }
                    }
                    break;
                case ETextAligment.eUnknown:
                    break;
            }


            OutputTextBox.Text += text;
        }

        public void MessageRecievedCallback(IntPtr ptr)
        {
            string text = Marshal.PtrToStringAnsi(ptr);
            ShowText(text, ETextAligment.eLeft);
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

                while (_threadDllMain.ThreadState != ThreadState.Stopped)
                {
                    Thread.Sleep(0);
                }
            }
            Application.Current.Shutdown();
        }

        private void InputTextBox_OnGotKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e)
        {
            InputTextBox.Clear();
        }
        private void MainWindow_OnMouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            ShowText("Test", ETextAligment.eRight);
        }
    }
}
