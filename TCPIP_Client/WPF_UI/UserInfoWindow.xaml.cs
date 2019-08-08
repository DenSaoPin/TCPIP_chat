using System;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Input;

namespace WPF_UI
{
    public partial class UserInfoWindow : Window
    {
        private MainWindow _mainWindow;
        public UserInfoWindow()
        {
            InitializeComponent();
            OkButton.IsEnabled = true;
            _mainWindow = (WPF_UI.MainWindow)Application.Current.MainWindow;
        }

        private static Regex _portRegex = new Regex("[0-9]+");
        private static Regex _nameRegex = new Regex("[A-Za-z 0-9_]+");
        private static Regex _ipRegex = new Regex("[A-Za-z0-9-.]+");

        private enum CheckType
        {
            eDefault,
            ePort,
            eName,
            eIP,
        }

        private static bool IsTextAllowed(string text, CheckType type)
        {
            switch (type)
            {
                case CheckType.ePort:
                    return _portRegex.IsMatch(text);
                case CheckType.eName:
                    return _nameRegex.IsMatch(text);
                case CheckType.eIP:
                    return _ipRegex.IsMatch(text);
                case CheckType.eDefault:
                    break;
            }

            return false;
        }

        private void TrashPastingPrevent(object sender, DataObjectPastingEventArgs e, CheckType type)
        {
            if (e.DataObject.GetDataPresent(typeof(String)))
            {
                String text = (String)e.DataObject.GetData(typeof(String));
                if (!IsTextAllowed(text, type))
                {
                    e.CancelCommand();
                }
            }
            else
            {
                e.CancelCommand();
            }
        }

        private void OkButton_Click(object sender, RoutedEventArgs e)
        {
            this.Hide();

            if (string.IsNullOrEmpty(NameTextBox.Text))
            {
                NameTextBox.Text = NameTextBox.WatermarkText;
            }
            if (string.IsNullOrEmpty(IP_TextBox.Text))
            {
                IP_TextBox.Text = IP_TextBox.WatermarkText;
            }
            if (string.IsNullOrEmpty(PortTextBox.Text))
            {
                PortTextBox.Text = PortTextBox.WatermarkText;
            }

            MainWindow.ClientInfo.Name = NameTextBox.Text;
            MainWindow.ClientInfo.Adress = IP_TextBox.Text;
            MainWindow.ClientInfo.Port = PortTextBox.Text;

            _mainWindow.NameLabel.Content = NameTextBox.Text;

            _mainWindow.Show();
            _mainWindow.MainLoop();
        }
        private void ExitButton_Click(object sender, RoutedEventArgs e)
        {
            _mainWindow.Button_Exit_OnClick(sender, e);
        }

        private void PortTextBox_OnPreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            e.Handled = !IsTextAllowed(e.Text, CheckType.ePort);
        }

        private void PortTextBox_OnPastingTextBoxPasting(object sender, DataObjectPastingEventArgs e)
        {
            TrashPastingPrevent(sender, e, CheckType.ePort);
        }

        private void NameTextBox_OnPreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            e.Handled = !IsTextAllowed(e.Text, CheckType.eName);
        }

        private void NameTextBox_OnPasting(object sender, DataObjectPastingEventArgs e)
        {
            TrashPastingPrevent(sender, e, CheckType.eName);
        }

        private void IP_TextBox_OnPreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            e.Handled = !IsTextAllowed(e.Text, CheckType.eName);
        }

        private void IP_TextBox_OnPasting(object sender, DataObjectPastingEventArgs e)
        {
            TrashPastingPrevent(sender, e, CheckType.eIP);
        }

    }
}
