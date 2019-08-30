using System;
using System.ComponentModel;
using System.Media;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Windows;
using System.Windows.Media;
using AttachedPropertyTest;

namespace WPF_UI
{
    public static class Native
    {
        public delegate void MessageRecievedCallbackDelegate([MarshalAs(UnmanagedType.LPStr)] string name, [MarshalAs(UnmanagedType.I4)] int type, [MarshalAs(UnmanagedType.LPStr)] string text);

        [DllImport("TCPIP_CLIENT_DLL.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern void ClientMainLoop();

        [DllImport("TCPIP_CLIENT_DLL.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern void setCallbackMessageReceived([MarshalAs(UnmanagedType.FunctionPtr)] MessageRecievedCallbackDelegate ptr);

        [DllImport("TCPIP_CLIENT_DLL.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern void ClientSendMessage([MarshalAs(UnmanagedType.LPStr)] string szStrName, [MarshalAs(UnmanagedType.I4)] int status, IntPtr dataPtr, [MarshalAs(UnmanagedType.I4)] int dataLenBytes);

        [DllImport("TCPIP_CLIENT_DLL.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern void ClientTerminate();

        [DllImport("TCPIP_CLIENT_DLL.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern EClientStatus GetClientDllStatus();

        [DllImport("TCPIP_CLIENT_DLL.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern void SetConnectionParams([MarshalAs(UnmanagedType.LPStr)] string name, [MarshalAs(UnmanagedType.LPStr)] string ip, [MarshalAs(UnmanagedType.LPStr)] string port);

        public enum EClientStatus
        {
            eInvalid          = 0,
            eStartWSA         = 1,
            eInitializeSocket = 2,
            eIntroduce        = 3,
            eIdle             = 4,
            eTerminating      = 5,
            eShutDown         = 6,
        };

        public enum EMessageType
        {
            eInvalid = 0x00,
            eNameRequest = 0x01,
            eDirectMessage = 0x05,
            eBroadcastMessage = 0x06,
            eResponse = 0x07,
        };
    }
    public static class ClientInfo
    {
        public static string Name;
        public static string Adress;
        public static string Port;
    }

    public partial class MainWindow : Window
    {
        private static Thread _threadDllMain;
        private static Native.EClientStatus _clientStatus = Native.EClientStatus.eInvalid;
        private static FlashWindowHelper _flashHelper;
        private Timer _statusChecker;

        public static Native.MessageRecievedCallbackDelegate _staticMRDelegate = MainWindow.MessageRecievedCallback;

        public MainWindow()
        {
            InitializeComponent();

            Hide();

            UserInfoWindow userInfoWindow = new UserInfoWindow();
            userInfoWindow.Show();
            TextBoxUtilities.SetAlwaysScrollToEnd(OutputTextBox, true);

            _flashHelper = new FlashWindowHelper(Application.Current);
        }

        public void MainLoop()
        {
            Native.SetConnectionParams(ClientInfo.Name, ClientInfo.Adress, ClientInfo.Port);
            Native.setCallbackMessageReceived(_staticMRDelegate);

            _threadDllMain = new Thread(ClientThreadFunc);
            _threadDllMain.Start();

            const int sec = 1000;

            Timer timer = new Timer(x =>
            {
                Dispatcher.Invoke(() =>
                {
                    _clientStatus = Native.GetClientDllStatus();
                    Status_Button.Background =
                        _clientStatus != Native.EClientStatus.eShutDown ? Brushes.Green : Brushes.Red;
                });
            },null, 0, sec);
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
            string text = InputTextBox.Text;
            if (!string.IsNullOrEmpty(text) && text != "...enter your message")
                ShowText(text, ETextAligment.eLeft);
            //TODO send message to selected Client

            if (string.IsNullOrEmpty(text))
                return;

            byte[] byteData = Encoding.ASCII.GetBytes(text);
            IntPtr ptr = Marshal.UnsafeAddrOfPinnedArrayElement(byteData, 0);
           
            Native.ClientSendMessage(ClientInfo.Name, (int)Native.EMessageType.eBroadcastMessage, ptr, byteData.Length);

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

        public static void MessageRecievedCallback(string sourceName, int messageType, string text)
        {
            Application.Current.Dispatcher.Invoke(() =>
            {
                MainWindow mw = Application.Current.MainWindow as MainWindow;
                if (mw == null)
                    throw new Exception("ALARM: Lost MainWindow ");

                mw.ShowText(sourceName + ": " + text, ETextAligment.eLeft);

                SoundPlayer player = new SoundPlayer(Properties.Resources.icq_sound);
                player.Play();

                _flashHelper.FlashApplicationWindow();
            });
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
            Close();
        }

        private void MainWindow_OnClosing(object sender, CancelEventArgs e)
        {
            if (_clientStatus != Native.EClientStatus.eShutDown || _clientStatus != Native.EClientStatus.eTerminating)
            {
                Native.ClientTerminate();

                while (_threadDllMain.ThreadState != ThreadState.Stopped)
                {
                    Thread.Sleep(0);
                }
            }

            _statusChecker.Change(Timeout.Infinite, Timeout.Infinite);

            Application.Current.Shutdown();
        }
    }
}
