﻿using System;
using System.Media;
using System.Runtime.InteropServices;
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
        public static extern void ClientSendMessage([MarshalAs(UnmanagedType.LPStr)] string szStr);

        [DllImport("TCPIP_CLIENT_DLL.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern void ClientTerminate();

        [DllImport("TCPIP_CLIENT_DLL.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern bool IsWorkingState();

        [DllImport("TCPIP_CLIENT_DLL.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern void SetConnectionParams([MarshalAs(UnmanagedType.LPStr)] string name, [MarshalAs(UnmanagedType.LPStr)] string ip, [MarshalAs(UnmanagedType.LPStr)] string port);
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
        private static Thread _threadStatusChecker;
        private static bool _clientExecuted = false;

        public static Native.MessageRecievedCallbackDelegate _staticMRDelegate = MainWindow.MessageRecievedCallback;
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
            Native.setCallbackMessageReceived(_staticMRDelegate);

            _threadDllMain = new Thread(ClientThreadFunc);
            _threadDllMain.Start();

            Timer timer = new Timer(x =>
            {
                Dispatcher.Invoke(() =>
                {
                    _clientExecuted = Native.IsWorkingState();
                    Status_Button.Background = _clientExecuted ? Brushes.Green : Brushes.Red;
                });
            });
            timer.Change(0, 1000);
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

        public static void MessageRecievedCallback(string sourceName, int messageType, string text)
        {
            Application.Current.Dispatcher.Invoke(() =>
            {
                MainWindow mw = Application.Current.MainWindow as MainWindow;
                if (mw == null)
                    throw new Exception("ALARM: Lost MainWindow ");

                mw.ShowText(sourceName + ": " + text, ETextAligment.eLeft);

                //SoundPlayer player = new SoundPlayer(Properties.Resources.icq);
                //player.Play();
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
    }
}
