using System;
using System.Diagnostics;
using System.Windows.Input;
using Xamarin.Forms;
using System.IO;
using static PicoXamarinDriver.ViewModels.MainViewModel;


namespace PicoXamarinDriver.ViewModels
{
    public class SettingsViewModel : BaseViewModel
    {
        #region myglobalssettings
        public class MyGlobalsSettings
        {
            public static string IP = "";
            public static string Port = "";
            public static string key = "";
        }
        #endregion
        public SettingsViewModel()
        {
            Title = "Browse";
            Locked = true;
            LockedKey = true;
            ServerAddress = "";
            ServerPort = "";
            FrameVisible = "False";

            btnSave = new Command(btnSaveAction);
            btnOk = new Command(btnOkAction);
    
            string[] arr = new string[2];
            if (!File.Exists(Path.Combine(Xamarin.Essentials.FileSystem.AppDataDirectory, "key.txt")))
            {
                File.Create(Path.Combine(Xamarin.Essentials.FileSystem.AppDataDirectory, "key.txt")).Close();
            }
            else
            {
                MyGlobalsSettings.key = File.ReadAllText(Path.Combine(Xamarin.Essentials.FileSystem.AppDataDirectory, "key.txt"));
                Key = MyGlobalsSettings.key;
            }

                if (!File.Exists(Path.Combine(Xamarin.Essentials.FileSystem.AppDataDirectory, "settings.txt")))
            {
                try
                {
                    File.Create(Path.Combine(Xamarin.Essentials.FileSystem.AppDataDirectory, "settings.txt")).Close();
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex);
                }

                try
                {
                    arr[0] = "127.0.0.1";
                    arr[1] = "80";
                    File.WriteAllLines(Path.Combine(Xamarin.Essentials.FileSystem.AppDataDirectory, "settings.txt"), arr);
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex);
                }
            }
            else
            {
                arr[0] = File.ReadAllLines(Path.Combine(Xamarin.Essentials.FileSystem.AppDataDirectory, "settings.txt"))[0];
                arr[1] = File.ReadAllLines(Path.Combine(Xamarin.Essentials.FileSystem.AppDataDirectory, "settings.txt"))[1];
            }
            MyGlobalsSettings.IP = arr[0];
            MyGlobalsSettings.Port = arr[1];
            ServerAddress= arr[0];
            ServerPort= arr[1];
            MyGlobals.serverPort = ServerPort;
            MyGlobals.serverAddress = ServerAddress;
            
        }

        private void btnOkAction(object obj)
        {
            FrameVisible = "False";
        }
        #region icommands
        public ICommand btnOk { get; }
        public ICommand btnSave { get; }
        #endregion
        #region button actions
        public async void btnSaveAction(object obj)
        {
            string[] arr = new string[2];
            arr[0] = ServerAddress;
            arr[1] = ServerPort;
            File.WriteAllLines(Path.Combine(Xamarin.Essentials.FileSystem.AppDataDirectory, "settings.txt"), arr);
            File.WriteAllText(Path.Combine(Xamarin.Essentials.FileSystem.AppDataDirectory, "key.txt"), Key);
            MyGlobalsSettings.key = Key;
            LockedKey = true;
            Locked = true;
            FrameVisible = "True";
        }
        #endregion
        public void readSettings()
        {
            string[] arr = new string[2];
            if (!File.Exists(Path.Combine(Xamarin.Essentials.FileSystem.AppDataDirectory, "settings.txt")))
            {

                File.Create(Path.Combine(Xamarin.Essentials.FileSystem.AppDataDirectory, "settings.txt"));
                try
                {
                    File.WriteAllLines(Path.Combine(Xamarin.Essentials.FileSystem.AppDataDirectory, "settings.txt"), arr);
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex);
                }
            }
            else
            {

                arr[0] = File.ReadAllLines(Path.Combine(Xamarin.Essentials.FileSystem.AppDataDirectory, "settings.txt"))[0];
                arr[1] = File.ReadAllLines(Path.Combine(Xamarin.Essentials.FileSystem.AppDataDirectory, "settings.txt"))[1];
                Console.WriteLine(arr[0] + arr[1]);
            }
            MyGlobals.serverAddress = arr[0];
            MyGlobals.serverPort = arr[1];
            Console.WriteLine("server: " + arr[0] + ", port: " + arr[1]);
        }

        #region public strings
        private string _popupVisible;
        public string FrameVisible
        {
            get { return _popupVisible; }
            set { SetProperty(ref _popupVisible, value); }
        }

        private string _popupText;
        public string PopupText
        {
            get { return _popupText; }
            set { SetProperty(ref _popupText, value); }
        }

        private string _serverAddress; 
        public string ServerAddress
        {
            get { return _serverAddress; }
            set { SetProperty(ref _serverAddress, value); }
        }
        private string _serverPort;
        public string ServerPort
        {
            get { return _serverPort; }
            set { SetProperty(ref _serverPort, value); }
        }

        private bool _locked;
        public bool Locked
        {
            get { return _locked; }
            set { SetProperty(ref _locked, value); }
        }

        private bool _lockedKey;
        public bool LockedKey
        {
            get { return _lockedKey; }
            set { SetProperty(ref _lockedKey, value); }
        }

        private string _key;
        public string Key
        {
            get { return _key; }
            set { SetProperty(ref _key, value); }
        }
        #endregion
    }
}