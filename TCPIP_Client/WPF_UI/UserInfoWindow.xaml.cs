using System;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Input;

namespace WPF_UI
{
    /// <summary>
    /// Interaction logic for UserInfoWindow.xaml
    /// </summary>
    public partial class UserInfoWindow : Window
    {
        public UserInfoWindow()
        {
            InitializeComponent();
        }

        private static Regex _portRegex = new Regex("[0-9]+");
        private static Regex _nameRegex = new Regex("[A-z 0-9_]+", RegexOptions.IgnoreCase);

        private enum CheckType
        {
            eDefault,
            ePort,
            eName,
        }

        private static bool IsTextAllowed(string text, CheckType type)
        {
            switch (type)
            {
                case CheckType.ePort:
                    return _portRegex.IsMatch(text);
                case CheckType.eName:
                    return _nameRegex.IsMatch(text);
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
            if (!string.IsNullOrEmpty(PortTextBox.Text))
            { 
            }

            MainWindow mainWindow = (WPF_UI.MainWindow)Application.Current.MainWindow;
            this.Hide();
            mainWindow.Show();
        }

        private void ExitButton_Click(object sender, RoutedEventArgs e)
        {

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
    }
}
