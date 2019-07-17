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

namespace WPF_UI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void button_SendMesage_OnClick(object sender, RoutedEventArgs e)
        {
            //TODO how to check which side used
            OutputTextBox.TextAlignment = TextAlignment.Left;

            OutputTextBox.Text = OutputTextBox.Text + "/n" + InputTextBox.Text;

            //TODO send message to selected Client

            InputTextBox.Clear();
            InputTextBox.Text = "...enter your message";
        }

        public void OutputTextBox_PrintText(string text)
        {          
            //TODO how to check which side used
            OutputTextBox.Text = OutputTextBox.Text + text;
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
    }

    //internal enum ETextSide
    //{
    //    eLeft,
    //    eRight,
    //}
}
